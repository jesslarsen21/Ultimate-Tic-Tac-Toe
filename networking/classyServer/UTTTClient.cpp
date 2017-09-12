#include "UTTTClient.h"
#include "UTTT.h"
#include "Messaging.h"
#include "logging.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include "AIPlayer.h"
#include "HumanPlayer.h"

static void my_perror(const char *msg)
{
	std::cerr << std::strerror(errno) << ": " << msg << std::endl;
	errno = 0;
}

UTTTClient::UTTTClient()
	: m_player(0),
		m_is_ai(false),
		m_have_updates(false),
		m_logged_in(false),
		m_game_over(false)
{
	initializePlayer();
	if(m_is_ai) { setUseStdin(false); }
}

UTTTClient::UTTTClient(bool is_ai)
	: m_player(0),
		m_is_ai(is_ai),
		m_have_updates(false),
		m_logged_in(false),
		m_game_over(false)
{
	initializePlayer();
	if(m_is_ai) { setUseStdin(false); }
}

UTTTClient::~UTTTClient()
{
	if(m_player)
		{
			delete m_player;
			m_player = 0;
		}
}

static char player_symbol(int player)
{
	if(player == PLAYER_X) { return 'X'; }
	else if(player == PLAYER_O) { return 'O'; }
	else if(player == TIE) { return 'T'; }
	else { return 'N'; }
}

static int player_from_symbol(char symbol)
{
	if(symbol == 'X') { return  PLAYER_X; }
	else if(symbol == 'O') { return  PLAYER_O; }
	else if(symbol == 'T') { return  TIE; }
	else { return PLAYER_N; }
}


int UTTTClient::handleServer()
{
	ssize_t nread = m_buffer.read(this->sock);
	LOG_STREAM(std::cerr) << "nread: " << nread << std::endl;
	if(nread > 0)
		{
			while(m_buffer.haveMessage())
				{
					std::vector<char> msg;
					std::vector<char> response;
					m_buffer.getMessage(msg);
					{
						std::string data;
						data.resize(msg.size());
						std::copy(msg.begin(), msg.end(), data.begin());
						LOG_STREAM(std::cerr) << "received: " << data << std::endl;
					}

					ssize_t n = buildResponse(msg, response);
					if( n < 0)
						{ // error in message
							LOG_STREAM(std::cerr) << "Unable to build response." << std::endl;
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
					else
						{ // This is a UTTT specific message handle here.
							Serialization::Serializer ser;
							std::string data, err;
							Messaging::IMsg* nmsg = 0;
							data.resize(msg.size() + 1);
							std::copy(msg.begin(), msg.end(), data.begin());
							if ((err = ser.Deserialize(data, nmsg)) != "")
								{
									LOG_STREAM(std::cerr) << "Deserialize Failure => " << err << std::endl;
									LOG_STREAM(std::cerr) << "Message => '" << data << "'" << std::endl;
                  if(nmsg)
                    {
                      delete nmsg;
                    }
									return -1;
								}
							if(nmsg == 0)
								{
									std::cerr << "nmsg == 0" << std::endl;
									return -1;
								}
							if(nmsg->GetTypeName() == Messaging::NextTurnMsg::GTypeName)
								{
									Messaging::NextTurnMsg* finMsg = dynamic_cast<Messaging::NextTurnMsg*>(nmsg);
									if (finMsg == NULL) {
										std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    delete nmsg;
										return -1;
									}
									m_player->setTurn(player_from_symbol(finMsg->GetPlayer()));
									m_player->setNextBoard(finMsg->GetNextBoard());
									//std::cout << finMsg->GetTypeName() << " " << finMsg->GetPlayer() << " " << finMsg->GetNextBoard() << std::endl;
									m_have_updates = true;
								}
							else if(nmsg->GetTypeName() == Messaging::WinStateMsg::GTypeName)
								{
									Messaging::WinStateMsg* finMsg = dynamic_cast<Messaging::WinStateMsg*>(nmsg);
									if (finMsg == NULL) {
										std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    delete nmsg;
										return -1;
									}
									std::cout << finMsg->GetTypeName() << " " << finMsg->GetPlayer() << std::endl;
									if(player_from_symbol(finMsg->GetPlayer()))
										{
											m_game_over = true;
										}
									m_have_updates = true;
								}
							else if(nmsg->GetTypeName() == Messaging::BoardStateMsg::GTypeName)
								{
									Messaging::BoardStateMsg* finMsg = dynamic_cast<Messaging::BoardStateMsg*>(nmsg);
									if (finMsg == NULL) {
										std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    delete nmsg;
										return -1;
									}
									
									m_player->setBoardWinner(finMsg->GetBoard(), player_from_symbol(finMsg->GetPlayer()));
									//std::cout << finMsg->GetTypeName() << " " << finMsg->GetPlayer() << " " << finMsg->GetBoard() << std::endl;
									m_have_updates = true;
								}
							else if(nmsg->GetTypeName() == Messaging::MarkerMsg::GTypeName)
								{
									Messaging::MarkerMsg* finMsg = dynamic_cast<Messaging::MarkerMsg*>(nmsg);
									if (finMsg == NULL) {
										std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    delete nmsg;
										return -1;
									}
									
									m_player->setValue(player_from_symbol(finMsg->GetPlayer()), finMsg->GetBoard(), finMsg->GetPosition());
									if(finMsg->GetPlayer() == 'X' || finMsg->GetPlayer() == 'O')
										{
											std::cout << finMsg->GetTypeName() << " " << finMsg->GetPlayer() << "(" << player_from_symbol(finMsg->GetPlayer()) << ") " << finMsg->GetBoard() << " " << finMsg->GetPosition() << std::endl;
										}
									m_have_updates = true;
								}
							else if(nmsg->GetTypeName() == Messaging::YouAreMsg::GTypeName)
								{
									Messaging::YouAreMsg* finMsg = dynamic_cast<Messaging::YouAreMsg*>(nmsg);
									if (finMsg == NULL) {
										std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    delete nmsg;
										return -1;
									}

									if(!m_player->setPlayer(player_from_symbol(finMsg->GetPlayer()))) {
										std::cerr << "Unable to setPlayer()" << std::endl;
									}
									//std::cout << finMsg->GetTypeName() << " " << finMsg->GetPlayer() << std::endl;
									m_have_updates = true;
								}
							else if(nmsg->GetTypeName() == Messaging::TurnFailedMsg::GTypeName)
								{
									Messaging::TurnFailedMsg* finMsg = dynamic_cast<Messaging::TurnFailedMsg*>(nmsg);
									if (finMsg == NULL) {
										std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    delete nmsg;
										return -1;
									}

									std::cout << finMsg->GetTypeName() << " " << finMsg->GetPlayer()
														<< " " << finMsg->GetMessage() << std::endl;
									m_have_updates = true;
								}
							else if(nmsg->GetTypeName() == Messaging::SignUpReplyMsg::GTypeName)
								{
									Messaging::SignUpReplyMsg* finMsg = dynamic_cast<Messaging::SignUpReplyMsg*>(nmsg);
									if (finMsg == NULL) {
										std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    delete nmsg;
										return -1;
									}

									std::cout << finMsg->GetTypeName() << " "
                            << finMsg->IsUsernameValid() << " "
                            << finMsg->IsEmailValid() << " "
                            << finMsg->GetPasswordState() << std::endl;
								}
							else if(nmsg->GetTypeName() == Messaging::LoginReplyMsg::GTypeName)
								{
									Messaging::LoginReplyMsg* finMsg = dynamic_cast<Messaging::LoginReplyMsg*>(nmsg);
									if (finMsg == NULL) {
										std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    delete nmsg;
										return -1;
									}
									if(finMsg->IsValid())
										{
											m_logged_in = true;
										}
									std::cout << finMsg->GetTypeName() << " "
                            << finMsg->IsValid() << " "
                            << finMsg->GetToken() << std::endl;
								}
							else if(nmsg->GetTypeName() == Messaging::OpponentNameMsg::GTypeName)
								{
									Messaging::OpponentNameMsg* finMsg = dynamic_cast<Messaging::OpponentNameMsg*>(nmsg);
									if (finMsg == NULL) {
										std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    delete nmsg;
										return -1;
									}
									m_opponent = finMsg->GetUsername();
									std::cout << finMsg->GetTypeName() << " "
                            << finMsg->GetUsername() << std::endl;
								}
							else if(nmsg->GetTypeName() == Messaging::UpdateFinishedMsg::GTypeName)
								{
									Messaging::UpdateFinishedMsg* finMsg = dynamic_cast<Messaging::UpdateFinishedMsg*>(nmsg);
									if (finMsg == NULL) {
										std::cerr << "Serializer lied about successful deserialize, type is incorrect." << std::endl;
                    delete nmsg;
										return -1;
									}
									std::cout << "Update Finished"
                            << std::endl;
								}
							else
								{
									std::cout << "Unhandled message type: " << nmsg->GetTypeName() << std::endl;
								}
							delete nmsg;
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

int UTTTClient::handleStdin()
{
	if(m_use_stdin)
		{
			std::string buf;
			std::getline(std::cin, buf);
			
			if(buf == "ping")
				{
					const Messaging::PingMsg* const rmsg = new Messaging::PingMsg();
					if(!sendMessage(rmsg)) {
						LOG_STREAM(std::cerr) << "Couldn't send ping message." << std::endl;
						delete rmsg;
						return -1;
					}
					delete rmsg;
				}
			else if(buf == "pong")
				{
					const Messaging::PongMsg* const rmsg = new Messaging::PongMsg();
					if(!sendMessage(rmsg)) {
						LOG_STREAM(std::cerr) << "Couldn't send pong message." << std::endl;
						delete rmsg;
						return -1;
					}
					delete rmsg;
				}
			else if(buf.substr(0,4) == "turn")
				{
					int board, position;
					std::stringstream ss;
					std::string command, playerstr;
					ss.str(buf);
					if(ss >> command >> playerstr >> board >> position)
						{
							if(!sendTurnMessage(playerstr[0], board, position))
								{
									return -1;
								}
						}
					else
						{
							std::cerr << "bad turn command: " << buf << std::endl;
						}
				}
			else if(buf == "whoami")
				{
					const Messaging::WhoamiMsg* const rmsg = new Messaging::WhoamiMsg();
					if(!sendMessage(rmsg)) {
						LOG_STREAM(std::cerr) << "Couldn't send whoami message." << std::endl;
						delete rmsg;
						return -1;
					}
					delete rmsg;
				}
			else if(buf.substr(0,5) == "login")
				{
					std::stringstream ss;
					std::string command, username, password;
					ss.str(buf);
					if(ss >> command >> username >> password)
						{
							if(!sendLoginMessage(username, password))
								{
									return -1;
								}
						}
					else
						{
							std::cerr << "bad login command: " << buf << std::endl;
						}
				}
			else if(buf.substr(0,6) == "signup")
				{
					std::stringstream ss;
					std::string command, username, email, password1, password2;
					ss.str(buf);
					if(ss >> command >> username >> email >> password1 >> password2)
						{
							const Messaging::SignUpMsg* const rmsg = new Messaging::SignUpMsg(username, email, password1, password2);
							if(!sendMessage(rmsg)) {
								LOG_STREAM(std::cerr) << "Couldn't send singup message." << std::endl;
								delete rmsg;
								return -1;
							}
							delete rmsg;
						}
					else
						{
							std::cerr << "bad signup command: " << buf << std::endl;
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
					std::cout << "Known: ping pong quit turn signup login" << std::endl;
				}
		}
	return 0;
}

bool UTTTClient::sendMessage(const Messaging::IMsg* const rmsg)
{
	Serialization::Serializer ser;

	std::string data, err;
	if ((err = ser.Serialize(rmsg, data)) != "")
		{
      LOG_STREAM(std::cerr) << "Serialize Failure => " << err << std::endl;
			return false;
		}
	LOG_STREAM(std::cerr) << "sending: " << data << std::endl;
	std::vector<char> msg(data.size());
	std::copy(data.begin(), data.end(), msg.begin());
	int n = sendServer(msg);
	if(n < 0)
		{
      LOG_STREAM(std::cerr) << std::strerror(errno) << std::endl
                            << "Error sending to server." << std::endl;
      closeSocket();
      return false;
		}
	return true;
}

void UTTTClient::initializePlayer()
{
	if(m_player)
		{
			delete m_player;
			m_player = 0;
		}
	if(m_is_ai)
		{
			m_player = new AIPlayer();
		}
	else
		{
			m_player = new HumanPlayer();
		}
}

bool UTTTClient::sendTurnMessage(char player, int board, int position)
{
	const Messaging::TurnMsg* const rmsg = new Messaging::TurnMsg(player, board, position);
	if(!sendMessage(rmsg))
		{
			LOG_STREAM(std::cerr) << "Couldn't send turn message." << std::endl;
			delete rmsg;
			return false;
		}
	delete rmsg;
	return true;
}

bool UTTTClient::sendLoginMessage(const std::string &username, const std::string &password)
{
	const Messaging::LoginMsg* const rmsg = new Messaging::LoginMsg(username, password);
	if(!sendMessage(rmsg))
		{
			LOG_STREAM(std::cerr) << "Couldn't send login message." << std::endl;
			delete rmsg;
			return false;
		}
	delete rmsg;
	return true;
}


int UTTTClient::heartbeat()
{
	if(m_have_updates)
		{
			if(m_is_ai)
				{
					if(m_player->isMyTurn())
						{
							int board, position;
							if(m_player->getMove(board, position))
								{
									if(!sendTurnMessage(player_symbol(m_player->getPlayer()), board, position))
										{
											return -1;
										}
								}
						}
				}
			else
				{
					std::cout << std::endl;
					std::cout << "Opponent: " << m_opponent << std::endl;
					m_player->showBoard();
					if(m_player->isMyTurn())
						{
							std::cout << "Move (turn " << player_symbol(m_player->getPlayer())
												<< " " << m_player->getNextBoard() << " ?) ? " << std::flush;
						}
				}
			if(m_game_over)
				{
					std::cout << "Game Over" << std::endl;
					this->closeSocket();
				}
			m_have_updates = false;
		}
	else
		{
			if(!m_logged_in && m_username != "" && m_password != "")
				{
					if(!sendLoginMessage(m_username, m_password))
						{
							return -1;
						}
				}
		}
	
	return 0;
}

bool UTTTClient::setCredentials(const std::string &username, const std::string &password)
{
	m_username = username;
	m_password = password;
	return true;
}
