#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <cstring>
#include <netinet/ip.h>
#include <sys/time.h>
#include <fcntl.h>
#include <ctime>
#include <cctype>
#include <cerrno>
#include <endian.h>
#include "Server.h"
#include <sstream>
#include <arpa/inet.h>
#include <iomanip>
#include "logging.h"



ConnectionClient::ConnectionClient()
	: sock(-1), state(STATE_TCP)
{
	if(gettimeofday(&m_last_time, 0) < 0)
		{
			DBGP_E(LOG_STREAM(std::cerr) << "gettimeofday failed." << std::endl);
		}
}

ConnectionClient::ConnectionClient(int state_in)
	: sock(-1), state(state_in)
{
	if(gettimeofday(&m_last_time, 0) < 0)
		{
			DBGP_E(LOG_STREAM(std::cerr) << "gettimeofday failed." << std::endl);
		}
}

ConnectionClient::~ConnectionClient()
{
	this->closeSocket();
}

int ConnectionClient::closeSocket()
{
	if(this->sock >= 0)
		{
			DBGP_SOCKET(LOG_STREAM(std::cout) << "closing socket " << *this << std::endl);
			
			if(shutdown(this->sock, SHUT_RDWR) < 0)
				{
					DBGP_E(LOG_STREAM(std::cerr) << "Couldn't shutdown socket, continue anyways." << std::endl);
				}
			if(close(this->sock) < 0)
				{
					DBGP_E(LOG_STREAM(std::cerr) << "Error closing client socket." << std::endl);
				}
			this->sock = -1;
		}
	return 0;
}

int ConnectionClient::forgetSocket()
{
  this->sock = -1;
  return 0;
}

int ConnectionClient::setTCP() {
	state = STATE_TCP | (state & STATE_NON_SOCK);
	return 0;
}

bool ConnectionClient::isTCP() const {
	return state & STATE_TCP;
}

int ConnectionClient::setWeb() {
	state = STATE_WEB | (state & STATE_NON_SOCK);
	return 0;
}

bool ConnectionClient::isWeb() const {
	return state & STATE_WEB;
}

int ConnectionClient::setWebWait() {
	state = STATE_WEB_WAIT | (state & STATE_NON_SOCK);
	return 0;
}

bool ConnectionClient::isWebWait() const {
	return state & STATE_WEB_WAIT;
}

int ConnectionClient::setWebFailed()
{
	state = STATE_WEB_FAILED | (state & STATE_NON_SOCK);
	return 0;
}
bool ConnectionClient::isWebFailed() const
{
	return state & STATE_WEB_FAILED;
}

int ConnectionClient::setState(int bits)
{
  state = state | (bits&STATE_NON_SOCK);
  return 0;
}
int ConnectionClient::clearState(int bits)
{
  state = state & ((~bits)&STATE_NON_SOCK);
  return 0;
}
bool ConnectionClient::isStateSet(int bits) const
{
	return state & (bits&STATE_NON_SOCK);
}

bool ConnectionClient::setData(const std::string &key, const std::string &value)
{
	data[key] = value;
	return true;
}
std::string ConnectionClient::getData(const std::string &key) const
{
	std::map<std::string, std::string>::const_iterator it = data.find(key);
	if(it == data.end())
		{
			return "";
		}
	return it->second;
}

	
ssize_t ConnectionClient::read()
{
	ssize_t n = -1;
	if(isTCP())
		{
			n = m_buffer.read(sock);
			DBGP_L(LOG_STREAM(std::cout) << "Read " << n << " bytes from TCP " << *this << std::endl);
		}
	else if(isWeb())
		{
			n = m_buffer.readWebSocketHandshake(sock);
			DBGP_L(LOG_STREAM(std::cout) << "Read " << n << " bytes from Web " << *this << std::endl);
		}
	else if(isWebWait())
		{
			n = m_buffer.readWebSocketFrame(sock);
			DBGP_L(LOG_STREAM(std::cout) << "Read " << n << " bytes from WebWait " << *this << std::endl);
		}
	else
		{
			DBGP_E(LOG_STREAM(std::cerr) << "Client state is unknown. " << *this << std::endl);
			n = -1;
		}
	if(n > 0)
		{
			if(gettimeofday(&m_last_time, 0) < 0)
				{
					DBGP_E(LOG_STREAM(std::cerr) << "gettimeofday failed." << std::endl);
				}
		}
	return n;
}

ssize_t ConnectionClient::write(const std::vector<char> &msg)
{
	ssize_t n = -1;
	if(isTCP())
		{
			n = m_buffer.write(sock, msg);
		}
	else if(isWeb())
		{
			n = m_buffer.writeWebSocketHandshake(sock, msg);
			if(n > 0)
				{
					setWebWait();
				}
		}
	else if(isWebWait())
		{
			n = m_buffer.writeWebSocketFrame(sock, msg);
		}
	else
		{
			DBGP_E(LOG_STREAM(std::cerr) << "Client state is unknown. " << *this << std::endl);
			n = -1;
		}
	return n;
}

std::string ConnectionClient::getAddress() const
{
	std::stringstream ss;
	char buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(addr.sin_addr), buf, INET_ADDRSTRLEN);
	ss << buf << ":" << ntohs(addr.sin_port);
	return ss.str();
}

double ConnectionClient::timeSinceLast() const
{
	double seconds, useconds;
	struct timeval now;
	if(gettimeofday(&now, 0) < 0)
		{
			DBGP_E(LOG_STREAM(std::cerr) << "gettimeofday failed." << std::endl);
			return -1.0;
		}
	seconds  = (now.tv_sec  - m_last_time.tv_sec)/1.0;
	useconds = (now.tv_usec - m_last_time.tv_usec)/1000000.;
	return seconds + useconds;
}

std::ostream &operator<<(std::ostream &os, const ConnectionClient &v)
{
	os << "Client[" << v.sock << "](" << std::hex << v.state << std::dec << "):" << v.getAddress();
	return os;
}


Server::Server()
	: tcp_sock(-1), web_sock(-1), clients(0),
		ws_validator("/")
{
	m_timeout.tv_sec  = 100;
	m_timeout.tv_usec = 0;
	initServer();
}

Server::~Server()
{
	this->closeServer();
}

int Server::initServer()
{
	this->tcp_sock		 = -1;
	this->web_sock		 = -1;
	this->clients.clear();
	return 0;
}

int Server::closeServer()
{
	//close all client sockets: clear uses ConnectionClient destructor
	size_t i;
	for (i=0; i < this->clients.size(); i++)
		{
			this->clients[i]->closeSocket();
			delete this->clients[i];
		}
	this->clients.clear();
	
	//close the server sockets
	if(this->tcp_sock >= 0)
		{
			if(close(this->tcp_sock) < 0)
				{
					DBGP_E(LOG_STREAM(std::cerr) << "Error closing tcp server socket" << std::endl);
				}
			this->tcp_sock = -1;
		}
	if(this->web_sock >= 0)
		{
			if(close(this->web_sock) < 0)
				{
					DBGP_E(LOG_STREAM(std::cerr) << "Error closing web server socket" << std::endl);
				}
			this->web_sock = -1;
		}
	return 0;
}

int Server::addClient(ConnectionClient *newClient)
{
	DBGP_SOCKET(LOG_STREAM(std::cout) << "adding client called on " << *newClient << std::endl);
	this->clients.push_back(newClient);
	return 0;
}

int Server::deleteClient(int which_client)
{
	DBGP_SOCKET(LOG_STREAM(std::cout) << "Deleting client called on " << which_client << std::endl);
	if(which_client < static_cast<int>(this->clients.size()) && which_client >= 0)
		{
			DBGP_SOCKET(LOG_STREAM(std::cout) << "Deleting client called on " << *(clients[which_client]) << std::endl);
			
			//Closes the client before deleting.
			this->clients[which_client]->closeSocket();
			delete this->clients[which_client];
			this->clients.erase(this->clients.begin() + which_client);
			
			return 0;
		}
	
	return -1;
}

int Server::countReadyClients() const
{
	int count = 0;
	size_t i;
	for(i = 0; i < clients.size(); i++)
		{
			if(clients[i]->isTCP() || clients[i]->isWebWait())
				{
					count ++;
				}
		}

	return count;
}

int Server::acceptClient(int which_server)
{
	ConnectionClient *c = new ConnectionClient(which_server == SERVER_TCP ?
																						 ConnectionClient::STATE_TCP :
																						 ConnectionClient::STATE_WEB );
	socklen_t len = sizeof(c->addr);
	int sock = which_server == SERVER_TCP ? this->tcp_sock : this->web_sock;
	if((c->sock = accept(sock, (struct sockaddr *)&(c->addr), &len)) < 0)
		{
			DBGP_E(LOG_STREAM(std::cerr) << (which_server == SERVER_TCP ? "TCP" : "WEB") << " "
						 << "accept failed" << std::endl);
			
			return -1;
		}
	
	int flags = fcntl(c->sock, F_GETFL, 0);
	if(fcntl(c->sock, F_SETFL, flags | O_NONBLOCK) < 0)
		{
			DBGP_E(LOG_STREAM(std::cerr) << "Set to nonblocking failed." << std::endl);

			return -1;
		}
	
	DBGP_SOCKET(LOG_STREAM(std::cout) << "new client connected: " << *c << std::endl);
	this->addClient(c);
	return 0;
}

int Server::prepareServer(int port, int backlog, int which_server)
{
	struct sockaddr_in *addr = which_server == SERVER_TCP ? &tcp_addr : &web_addr;
	int *sock = which_server == SERVER_TCP ? &tcp_sock : &web_sock;
  addr->sin_family      = AF_INET;
  addr->sin_port        = htons((unsigned short)port);
  addr->sin_addr.s_addr = INADDR_ANY;
  
  if((*sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP)) < 0)
    {
			DBGP_E(LOG_STREAM(std::cerr) << (which_server == SERVER_TCP ? "TCP" : "WEB") << " "
						 << "Can't create socket!" << std::endl);
			
      return -1;
    }

	int reuse_value = 1;
	if(setsockopt(*sock, SOL_SOCKET, SO_REUSEADDR, &reuse_value, sizeof(reuse_value)) < 0)
		{
			DBGP_E(LOG_STREAM(std::cerr) << (which_server == SERVER_TCP ? "TCP" : "WEB") << " "
						 << "Can't set REUSEADDR on socket." << std::endl);
			
			return -1;
		}
		
  if(bind(*sock, (struct sockaddr *)addr, sizeof(*addr)) != 0)
    {
			DBGP_E(LOG_STREAM(std::cerr) << (which_server == SERVER_TCP ? "TCP" : "WEB") << " "
						 << "Can't bind socket!" << std::endl);
			
      return -1;
    }
  
  if(listen(*sock, backlog) != 0)
    {
			DBGP_E(LOG_STREAM(std::cerr) << (which_server == SERVER_TCP ? "TCP" : "WEB") << " "
						 << "Can't listen socket!" << std::endl);
			
      return -1;
    }
  
  return 0;

}

// echo server response
int Server::buildResponse(ConnectionClient *client, int i, const std::vector<char> &msg,
													std::vector<char> &response)
{
	static const char *prefix = "ES: ";
	int n = 0;
	if(client->isTCP() || client->isWebWait())
		{
			response.clear();
			response.resize(msg.size() + std::strlen(prefix));
			std::copy(prefix, prefix + strlen(prefix), response.begin());
			std::copy(msg.begin(), msg.end(), response.begin() + strlen(prefix));
		}
	else if(client->isWeb())
		{
			const std::string err = ws_validator.Validate(msg, response);
			if(err != "")
				{
					DBGP_E(LOG_STREAM(std::cerr) << "Invalid handshake" << err << std::endl);
					
					client->setWebFailed();
				}
		}
	else
		{
			DBGP_E(LOG_STREAM(std::cerr) << "Client state is unknown." << std::endl);
			DBGP_SOCKET(LOG_STREAM(std::cerr) << "Closing socket " << i << std::endl);
			
			client->closeSocket();
			this->deleteClient(i);
		}
	return n;
}

int Server::handleClient(ConnectionClient *client, int i)
{
	ssize_t n = 0;
	
	n = client->read();
	
	if(n > 0)
		{
			while(client->m_buffer.haveMessage())
				{
					std::vector<char> msg;
					std::vector<char> response;
					client->m_buffer.getMessage(msg);
					
					n = buildResponse(client, i, msg, response);
					
					if(n < 0)
						{
							DBGP_E(LOG_STREAM(std::cerr) << "Bad response built." << std::endl);
							DBGP_SOCKET(LOG_STREAM(std::cerr) << "Closing socket (bad response) " << i << std::endl);
							
							client->closeSocket();
							this->deleteClient(i);
							return 0;
						}
					
					DBGP_SOCKET(LOG_STREAM(std::cerr) << response << std::endl);
					n = client->write(response);
					
					if(n < 0)
						{
							DBGP_SOCKET(LOG_STREAM(std::cerr) << "Closing socket (write failed) " << i << std::endl);
							
							client->closeSocket();
							this->deleteClient(i);
							return 0;
						}
				}
		}
	else if(n == 0)
		{
			/* client closed their side of the socket */
			DBGP_SOCKET(LOG_STREAM(std::cerr) << "Closing socket (client closed) " << i << std::endl);

			client->closeSocket();
			this->deleteClient(i);
		}
	else
		{
			/* read failed, client in unexpected state, etc. */
			DBGP_SOCKET(LOG_STREAM(std::cerr) << "Closing socket (bad read) " << i << std::endl);
			
			client->closeSocket();
			this->deleteClient(i);
		}
	return 0;
}

int Server::heartbeat(bool timedout)
{
	return 0;
}

int Server::mainLoop()
{
	int nfds = getdtablesize();
	fd_set rfds;
	size_t i;
	struct timeval time_out;
	int n;
	int found;
	
	while(tcp_sock >= 0 || web_sock >= 0 || clients.size() > 0)
		{
			/* Mark file descriptors we want to monitor */
			FD_ZERO(&rfds);
			if(tcp_sock >= 0)
				{
					DBGP_SOCKET(LOG_STREAM(std::cout) << "tcp server socket " << tcp_sock << std::endl);
					
					FD_SET(tcp_sock, &rfds);
				}
			if(web_sock >= 0)
				{
					DBGP_SOCKET(LOG_STREAM(std::cout) << "web server socket " << web_sock << std::endl);
					
					FD_SET(web_sock, &rfds);
				}
			for(i = 0; i < clients.size(); i++)
				{
					DBGP_SOCKET(LOG_STREAM(std::cout) << "client socket " << *clients[i] << std::endl);
					
					FD_SET(clients[i]->sock, &rfds);
				}

			/* Mark the time out period, if nothing is happening */
			time_out = m_timeout;
			
			/* Wait until action occurs on file descriptor(s) or time out is reached */
			if((n = select(nfds, &rfds, 0, 0, &time_out)) < 0)
				{
					DBGP_E(LOG_STREAM(std::cerr) << "Bad select on socket!" << std::endl);
					
					return -1;
				}
			
			if(n == 0)
				{
					/* time out */
					if(heartbeat(true) < 0)
						{
							DBGP_E(LOG_STREAM(std::cerr) << "heartbeat failed" << std::endl);
							
							return -1;
						}
				}
			else if(FD_ISSET(tcp_sock, &rfds))
				{
					/* accept a new client connection */
					acceptClient(SERVER_TCP);
				}
			else if(FD_ISSET(web_sock, &rfds))
				{
					/* accept a new client connection */
					acceptClient(SERVER_WEB);
				}
			else
				{
					/* incoming data from a client socket */
					found = 0;
					for(i = 0; i < clients.size(); i++)
						{
							if(FD_ISSET(clients[i]->sock, &rfds))
								{
									/* client[i] client has data */
									DBGP_L(LOG_STREAM(std::cout) << "Select found on " << *clients[i] << std::endl);
									found = 1;
									
									handleClient(clients[i], i);
								}
						}
					if(!found)
						{
							DBGP_E(LOG_STREAM(std::cerr) << "Select on unknown file descriptor! " << std::endl);
							
							return -1;
						}
				}
			if(heartbeat(false) < 0)
				{
					DBGP_E(LOG_STREAM(std::cerr) << "heartbeat failed" << std::endl);

					return -1;
				}
		}
	return 0;
}

int Server::setTimeOut(const struct timeval &timeout)
{
	m_timeout = timeout;
	return 0;
}
