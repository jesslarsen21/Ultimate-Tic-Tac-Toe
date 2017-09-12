#include "Client.h"
#include "fileno.h"
#include "logging.h"
#include <sys/select.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>

static void my_perror(const char *msg)
{
	std::cerr << std::strerror(errno) << ": " << msg << std::endl;
	errno = 0;
}
	
Client::Client()
	: sock(-1), server_name(""), server_port(-1), m_use_stdin(true)
{
}

Client::Client(const std::string &server_name_in, const int server_port_in)
	: sock(-1), server_name(server_name_in), server_port(server_port_in), m_use_stdin(true)
{
	this->prepareSocket(server_name_in, server_port_in);
}

Client::~Client()
{
	this->closeSocket();
}

int Client::prepareSocket(const std::string &server_name_in, const int server_port_in)
{
	this->server_name = server_name_in;
	this->server_port = server_port_in;

	struct hostent *host_ent;

	/* lookup IP address of server */
	host_ent = gethostbyname(this->server_name.c_str());
	if(host_ent == 0)
		{
			my_perror("Could not lookup host by name");
			return -1;
		}

	/* format the server address for system calls */
	this->server_addr.sin_family      = AF_INET;
	this->server_addr.sin_port        = htons((unsigned short)this->server_port);
	this->server_addr.sin_addr.s_addr = *((u_int32_t *)host_ent->h_addr_list[0]);

	/* create the socket */
	if((this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		{
			my_perror("Can't Create Socket");
			return -1;
		}

	/* connect to the server */
	if(connect(this->sock, (struct sockaddr *)&(this->server_addr), sizeof(this->server_addr)) < 0)
		{
			if(errno == ECONNREFUSED)
				{
					my_perror("Connection Refused");
				}
			else if(errno == ENETUNREACH)
				{
					my_perror("Network Unreachable");
				}
			else if(errno == ETIMEDOUT)
				{
					my_perror("Connection Timed Out");
				}
			else if(errno == EHOSTUNREACH)
				{
					my_perror("Host Unreachable");
				}
			else
				{
					my_perror("Could not connect to host.");
				}
			this->closeSocket();
			return -1;
		}

	int flags = fcntl(this->sock, F_GETFL, 0);
	if(fcntl(this->sock, F_SETFL, flags | O_NONBLOCK) < 0)
		{
			my_perror("Set to nonblocking failed.");
			return -1;
		}
	
	return 0;
}

int Client::getSocketFd() const
{
	return this->sock;
}

int Client::closeSocket()
{
	if(this->sock >= 0)
		{
			if(shutdown(this->sock, SHUT_RDWR) < 0)
				{
					my_perror("Couldn't shutdown socket, continue anyways.");
				}
			if(close(this->sock) < 0)
				{
					my_perror("Couldn't close socket, continue anyways.");
				}
			this->sock = -1;
		}
	return 0;
}

int Client::mainLoop()
{
	int n, nfds = getdtablesize();
	fd_set rfds;
	struct timeval time_out;
	
	while(this->sock >= 0) {
		FD_ZERO(&rfds);
		FD_SET(this->sock, &rfds);
		if(m_use_stdin)
			{
				FD_SET(fileno(std::cin), &rfds);
			}
		
		/* Mark the time out period, if nothing is happening */
		time_out.tv_sec  = 0;
		time_out.tv_usec = 100000; // .1 sec

		if((n = select(nfds, &rfds, 0, 0, &time_out)) < 0)
			{
				my_perror("Bad select on socket or keyboard!");
				return -1;
			}
		//LOG_STREAM(std::cerr) << "select: " << n << std::endl;

		if(n == 0)
			{ /* time out */
				//LOG_STREAM(std::cerr) << "heartbeat" << std::endl;
				if(heartbeat() < 0)	{
					my_perror("heartbeat failed");
					return -1;
				}
			}
		else
			{
				if(FD_ISSET(this->sock, &rfds))
					{
						LOG_STREAM(std::cerr) << "socket traffic" << std::endl;
						if(this->handleServer() < 0)
							{
								LOG_STREAM(std::cerr) << "Error handling server" << std::endl;
								return -1;
							}
					}
				else if(m_use_stdin && FD_ISSET(fileno(std::cin), &rfds))
					{
						LOG_STREAM(std::cerr) << "stdin traffic" << std::endl;
						if(this->handleStdin() < 0)
							{
								my_perror("Error handling stdin");
								return -1;
							}
					}
				else
					{
						my_perror("Error: no expected socket set.");
						return -1;
					}
			}
	}

	return 0;
}

int Client::sendServer(const std::vector<char> &buf)
{
	if(buf.size() > 0)
		{
			ssize_t nwritten = m_buffer.write(this->sock, buf);
			if(nwritten < 0)
				{
					my_perror("Error writing to server.");
					return -1;
				}
			else if(nwritten < static_cast<ssize_t>(buf.size()))
				{
					std::cerr << "nwritten: " << nwritten << " buf.size(): " << buf.size() << std::endl;
					my_perror("Not all bytes were written to the server.");
					return -1;
				}
		}
	return 0;
}

int Client::handleServer()
{
	ssize_t nread = m_buffer.read(this->sock);
	if(nread > 0)
		{
			while(m_buffer.haveMessage())
				{
					std::vector<char> msg;
					m_buffer.getMessage(msg);  msg.push_back(0);
					std::cout << "EC: " << msg.data() << std::endl;
				}
		}
	else if(nread == 0)
		{ /* A zero-byte read from the server, means the server shutdown its socket. */
			this->closeSocket();
			return 0;
		}
	else
		{
			my_perror("Error reading from the server.");
			return -1;
		}
	return 0;
}

int Client::handleStdin()
{
	if(m_use_stdin)
		{
			std::string buf;
			std::getline(std::cin, buf);
			std::vector<char> msg(buf.size());
			std::copy(buf.begin(), buf.end(), msg.begin());
			if(this->sendServer(msg) < 0)
				{
					my_perror("Error sending to server");
					return -1;
				}
		}
	return 0;
}

int Client::heartbeat()
{
	return 0;
}

int Client::setUseStdin(bool use_stdin)
{
	m_use_stdin = use_stdin;
	return 0;
}
