#include "MessagingClient.h"
#include "Messaging.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

static void my_perror(const char *msg)
{
	std::cerr << std::strerror(errno) << ": " << msg << std::endl;
	errno = 0;
}

MessagingClient::MessagingClient()
{
}

MessagingClient::~MessagingClient()
{
}

int MessagingClient::handleServer()
{
	ssize_t nread = m_buffer.read(this->sock);
	if(nread > 0)
		{
			while(m_buffer.haveMessage())
				{
					std::vector<char> msg;
					std::vector<char> response;
					m_buffer.getMessage(msg);

					ssize_t n = buildResponse(msg, response);
					if( n < 0)
						{ // error in message
							my_perror("Unable to build response.");
							this->closeSocket();
							return -1;
						}
					if(response.size() > 0)
						{
							if(sendServer(response) < 0)
								{
									my_perror("Error sending to server.");
									this->closeSocket();
									return -1;
								}
						}
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
			this->closeSocket();
			return -1;
		}
	return 0;
}

int MessagingClient::buildResponse(const std::vector<char> &msg, std::vector<char> &response)
{
	Serialization::Serializer ser;
			
	std::string data, err;
	Messaging::IMsg* nmsg = NULL;
	data.resize(msg.size() + 1);
	std::copy(msg.begin(), msg.end(), data.begin());
	if ((err = ser.Deserialize(data, nmsg)) != "")
		{
			std::cerr << "Deserialize Failure => " << err << std::endl;
			std::cerr << "Message => '" << data << "'" << std::endl;
			return -1;
		}
	//std::cerr << "Deserialized" << std::endl;


	if(nmsg->GetTypeName() == Messaging::PingMsg::GTypeName)
		{
			std::cerr << "Ping" << std::endl;
			/* construct and send Pong */
					
			// write(fileno(stdout), "ping: ```", 9);
			// write(fileno(stdout), "'''\n", 4);
					
			const Messaging::PongMsg* const rmsg = new Messaging::PongMsg();
			if ((err = ser.Serialize(rmsg, data)) != "")
				{
					std::cerr << "Serialize Failure => " << err << std::endl;
					return -1;
				}
			response.clear();			
			response.resize(data.size());
			std::copy(data.begin(), data.end(), response.begin());
			delete rmsg;
		}
	else if(nmsg->GetTypeName() == Messaging::PongMsg::GTypeName)
		{
			std::cerr << "Pong" << std::endl;
			/* OK, no action */
					
			// write(fileno(stdout), "pong: ```", 9);
			// write(fileno(stdout), "'''\n", 4);
		}
	else
		{
			// std::cerr << "Other: " << nmsg->GetTypeName() << std::endl;
		}
	delete nmsg;
	return 0;
}

int MessagingClient::handleStdin()
{
	Serialization::Serializer ser;
	std::string data, err;
	std::vector<char> msg;
		
	std::string buf;
	std::getline(std::cin, buf);

	if(buf == "ping")
		{
			const Messaging::PingMsg* const rmsg = new Messaging::PingMsg();
			if ((err = ser.Serialize(rmsg, data)) != "")
				{
					std::cerr << "Serialize Failure => " << err << std::endl;
					return -1;
				}
			msg.clear();			
			msg.resize(data.size());
			std::copy(data.begin(), data.end(), msg.begin());
			delete rmsg;
			
			if(sendServer(msg) < 0)
				{
					my_perror("Error sending to server.");
					this->closeSocket();
					return -1;
				}
		}
	else if(buf == "pong")
		{
			const Messaging::PongMsg* const rmsg = new Messaging::PongMsg();
			if ((err = ser.Serialize(rmsg, data)) != "")
				{
					std::cerr << "Serialize Failure => " << err << std::endl;
					return -1;
				}
			msg.clear();			
			msg.resize(data.size());
			std::copy(data.begin(), data.end(), msg.begin());
			delete rmsg;
			
			if(sendServer(msg) < 0)
				{
					my_perror("Error sending to server.");
					this->closeSocket();
					return -1;
				}
		}
	else if(buf == "quit")
		{
			std::cout << "Goodbye." << std::endl;
			return -1;
		}
	else
		{
			std::cout << "Unknown command: " << buf << std::endl;
			std::cout << "Known: ping pong quit" << std::endl;
		}
	return 0;
}
