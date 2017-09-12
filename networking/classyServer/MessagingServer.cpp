#include "Messaging.h"
#include "MessagingServer.h"
#include "logging.h"
#include <cstring>
#include <algorithm>

#define DEBUG 0

#if DEBUG
#define DEBUG_PRINT_BUFFER(msg, buffer, size) { std::cerr << msg << "||"; if(size >= 0) { debug_print_buffer(std::cerr, buffer, size); } else { std::cerr << " size: " << size << std::endl; } }

static void debug_print_buffer(std::ostream &os, const char *buffer, const ssize_t size)
{
	ssize_t i;
	os << "BUFFER @" << (void *)buffer << " size: " << size << "::";
	for(i = 0; i < size; i++)
		{
			os << buffer[i];
		}
	os << "::" << std::endl;
}

#else
#define DEBUG_PRINT_BUFFER(msg, buffer, size) { }
#endif

#define MESSAGE_IS(nmsg, msgtype) ((nmsg)->GetTypeName() == Messaging::msgtype::GTypeName)
#define SET_FINMSG(nmsg, msgtype)                                       \
  Messaging::msgtype *finMsg = dynamic_cast<Messaging::msgtype *>((nmsg)); \
  {                                                                     \
		if(finMsg == NULL)																									\
			{																																	\
				DBGP_E(LOG_STREAM(std::cerr) << "Serializer lied about successful deserialize, type is incorrect for " \
							 << #msgtype << std::endl);																\
				if(nmsg) { delete nmsg; }																				\
				return -1;																											\
			}																																	\
  }

MessagingServer::MessagingServer()
	: Server()
{
}

MessagingServer::~MessagingServer()
{
}

int MessagingServer::handleClient(ConnectionClient *client, int i)
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

					if(response.size() > 0)
						{
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

int MessagingServer::buildResponse(ConnectionClient *client, int i, const std::vector<char> &msg,
																	 std::vector<char> &response)
{
	int n = 0;
	if(client->isTCP() || client->isWebWait())
		{
			/* Here:
			 * 1- deserialize the message
			 * 2- switch on message type
			 * 3- custom handler for each message type
			 * 4- do the client->write() as appropriate
			 *    a- some messages may require writes to many clients
			 * 5- clear response, so handleClient doesn't use it.
			 */
			Serialization::Serializer ser;
			
			std::string data, err;
			Messaging::IMsg* nmsg = NULL;
			data.resize(msg.size() + 1);
			std::copy(msg.begin(), msg.end(), data.begin());
			if ((err = ser.Deserialize(data, nmsg)) != "")
				{
					DBGP_E(LOG_STREAM(std::cerr) << "Deserialize Failure => " << err << std::endl);
					DBGP_E(LOG_STREAM(std::cerr) << "Message => '" << data << "'" << std::endl);
					return -1;
				}

			if(nmsg->GetTypeName() == Messaging::PingMsg::GTypeName)
				{
					SET_FINMSG(nmsg, PingMsg);
					DBGP_GAME(LOG_STREAM(std::cout) << *finMsg << std::endl);
					
					const Messaging::PongMsg* const rmsg = new Messaging::PongMsg();
					if ((err = ser.Serialize(rmsg, data)) != "")
						{
							DBGP_E(LOG_STREAM(std::cerr) << "Serialize Failure => " << err << std::endl);
							return -1;
						}
					response.clear();			
					response.resize(data.size());
					std::copy(data.begin(), data.end(), response.begin());

					DBGP_GAME(LOG_STREAM(std::cout) << response << std::endl);
					n = client->write(response);
					delete rmsg;
				}
			else if(nmsg->GetTypeName() == Messaging::PongMsg::GTypeName)
				{
					SET_FINMSG(nmsg, PongMsg);
					DBGP_GAME(LOG_STREAM(std::cout) << *finMsg << std::endl);
				}
			else
				{
				}
			delete nmsg;
			response.clear();
		}
	else if(client->isWeb())
		{
			/* Here:
			 * 1- create proper handshake message and put in response.
			 * 2- let handleClient send it.
			 */
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
