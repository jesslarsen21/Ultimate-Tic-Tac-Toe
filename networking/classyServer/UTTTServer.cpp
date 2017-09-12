#include "UTTTServer.h"
#include "logging.h"
#include "Messaging.h"
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <ctime>
#include <sstream>

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

#define K_USERNAME "kUserName"

UTTTServer::UTTTServer()
	: MessagingServer(),
		m_accounts(0),
		m_game(0),
		m_sent_you_are(false),
		m_game_id(-1)
{
	//m_accounts = new DB("uttt-accounts.db");
	m_accounts = new DB();
	
	// Need timeout poll to be shorter so AI autospawn can work in a timely fashion
	struct timeval timeout;
	timeout.tv_sec  = 30;
	timeout.tv_usec =  0;
	setTimeOut(timeout);
}

UTTTServer::~UTTTServer()
{
	if(m_game)
		{
			delete m_game;
			m_game = 0;
		}
	if(m_accounts)
		{
			m_accounts->close();
			delete m_accounts;
			m_accounts = 0;
		}
}

static int player_from_index(int i)
{
	switch(i)
		{
		case 0:
			return PLAYER_X;
			break;
		case 1:
			return PLAYER_O;
			break;
		default:
			return PLAYER_N;
			break;
		}
	return PLAYER_N;
}

static bool correct_player(int player, int i)
{
	return player_from_index(i) == player;
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



int UTTTServer::handleClient(ConnectionClient *client, int i)
{
	ssize_t n = 0;

	n = client->read();
	DBGP_L(LOG_STREAM(std::cout) << "Read " << n << " bytes from " << *client << std::endl);
	
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
							DBGP_SOCKET(LOG_STREAM(std::cerr) << "Closing client " << i << std::endl);
							
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
									DBGP_SOCKET(LOG_STREAM(std::cerr) << "Closing client (write failed) " << i << std::endl);
									
									client->closeSocket();
									this->deleteClient(i);
									return 0;
								}
						}
					else
						{ // This is a UTTT specific message. Handle here.
				
							Serialization::Serializer ser;
							std::string data, err;
							Messaging::IMsg* nmsg = 0;
							data.resize(msg.size() + 1);
							std::copy(msg.begin(), msg.end(), data.begin());
							if ((err = ser.Deserialize(data, nmsg)) != "")
								{
									DBGP_E(LOG_STREAM(std::cerr) << "Deserialize Failure => " << err << std::endl);
									DBGP_E(LOG_STREAM(std::cerr) << "Message => '" << data << "'" << std::endl);
									
									if(nmsg)
										{
											delete nmsg;
										}
									return -1;
								}
							if(nmsg == 0)
								{
									DBGP_E(LOG_STREAM(std::cerr) << "nmsg == 0" << std::endl);
									
									return -1;
								}
							
							if(m_game)
								{ // messages that require a game are inside here

									if(MESSAGE_IS(nmsg, TurnMsg))
										{
											SET_FINMSG(nmsg, TurnMsg);

											DBGP_GAME(LOG_STREAM(std::cout) << *finMsg << std::endl);
											
											if(!correct_player(player_from_symbol(finMsg->GetPlayer()), i))
												{
													DBGP_L(LOG_STREAM(std::cerr) <<  "Incorrect player." << std::endl);
													
													if(!sendTurnFailed(client, i, "not_your_turn"))
														{
															DBGP_E(LOG_STREAM(std::cerr) << "sendTurnFailed failed." << std::endl);
														}
												}							
											else if(!m_game->takeTurn(player_from_symbol(finMsg->GetPlayer()), finMsg->GetBoard(), finMsg->GetPosition()))
												{
													DBGP_L(LOG_STREAM(std::cerr) <<  "Bad Move." << std::endl);
													
													if(!sendTurnFailed(client, i, "bad_move"))
														{
															DBGP_E(LOG_STREAM(std::cerr) << "sendTurnFailed failed." << std::endl);
														}
												}
											else
												{
													DBGP_GAME(LOG_STREAM(std::cout) << "Move successful." << std::endl);
												}
										}
									else if(MESSAGE_IS(nmsg, WhoamiMsg))
										{
											SET_FINMSG(nmsg, WhoamiMsg);
											DBGP_GAME(LOG_STREAM(std::cout) << *finMsg << std::endl);
											
											if(!sendYouAre(client, i))
												{
													DBGP_E(LOG_STREAM(std::cerr) << "sendYouAre failed on client " << i << std::endl);
												}
											else
												{
													DBGP_GAME(LOG_STREAM(std::cout) << "YouAre." << std::endl);
												}
										}
								}
							else // !m_game
								{ // messages that don't require a game
									if(MESSAGE_IS(nmsg, LoginMsg))
										{
											SET_FINMSG(nmsg, LoginMsg);
											DBGP_GAME(LOG_STREAM(std::cout) << *finMsg << std::endl);

											bool rc = false;
											if(m_accounts)
												{
													rc = m_accounts->login(finMsg->GetEmail(), finMsg->GetPassword());
												}
										
											const Messaging::LoginReplyMsg* rmsg = new Messaging::LoginReplyMsg(rc, "xyz");
											if(!sendMessage(client, rmsg))
												{
													DBGP_E(LOG_STREAM(std::cerr) << "sendMessage failed for LoginReplyMsg" << std::endl);
													
													if(rmsg) delete rmsg;
													if(nmsg) delete nmsg;
													return -1;
												}
											
											delete rmsg;
											if(rc)
												{
													DBGP_GAME(LOG_STREAM(std::cout) << "Logged in " << finMsg->GetEmail() << std::endl);
													
													client->setState(STATE_LOGGED_IN);
													client->setData(K_USERNAME, finMsg->GetEmail());
												}
											else
												{
													DBGP_GAME(LOG_STREAM(std::cout) << "Not logged in " << finMsg->GetEmail() << std::endl);
													
													client->clearState(STATE_LOGGED_IN);
												}
										}
									else if(MESSAGE_IS(nmsg, SignUpMsg))
										{
											SET_FINMSG(nmsg, SignUpMsg);
											DBGP_GAME(LOG_STREAM(std::cout) << *finMsg << std::endl);

											int rc = 0;
											if(m_accounts && (finMsg->GetPassword() == finMsg->GetPasswordConfirm()))
												{
													rc = m_accounts->addUser(finMsg->GetUsername(), finMsg->GetPassword(), finMsg->GetEmail());
												}

											Messaging::SignUpReplyMsg::PasswordState pass_ok =
												((finMsg->GetPassword() != finMsg->GetPasswordConfirm()) ?
												 Messaging::SignUpReplyMsg::Unmatched :
												 ((rc & PASS_OK) ?
													Messaging::SignUpReplyMsg::Valid :  Messaging::SignUpReplyMsg::Invalid));
											const Messaging::SignUpReplyMsg* rmsg =
												new Messaging::SignUpReplyMsg(rc & USER_OK, rc & EMAIL_OK, pass_ok);
											if(!sendMessage(client, rmsg))
												{
													DBGP_E(LOG_STREAM(std::cerr) << "sendMessage failed for SignUpReplyMsg" << std::endl);
													if(rmsg) delete rmsg;
													if(nmsg) delete nmsg;
													return -1;
												}
											
											delete rmsg;
										}
									else
										{
											DBGP_E(LOG_STREAM(std::cerr)
														 << "Unexpected non-game message: "
														 << nmsg->GetTypeName() << " from " << (*client)
														 << std::endl);
										}
								}
							
							delete nmsg;
						}
				}
		}
	else if(n == 0)
		{
			/* client closed their side of the socket */
			DBGP_L(LOG_STREAM(std::cout) << "Empty read: " << *client << std::endl);
			//DBGP_L(LOG_STREAM(std::cout) << "Not closing it though: " << *client << std::endl);
			handleClientClosed(client, i);
		}
	else
		{
			/* read failed, client in unexpected state, etc. */
			DBGP_L(LOG_STREAM(std::cout) << "Error read: " << *client << std::endl);
			handleClientClosed(client, i);
		}
	return 0;
}


int UTTTServer::heartbeat(bool timedout)
{
	DBGP_L(LOG_STREAM(std::cout) << "timedout: " << timedout << std::endl);

	int i;
	for(i = ((int)clients.size()) - 1; i >= 0; i--)
		{
			if(clients[i]->timeSinceLast() > 1800.) // 30 minutes
				{
					DBGP_SOCKET(LOG_STREAM(std::cerr) << "Timeout client " << i << std::endl);
							
					deleteClient(i);
				}
		}

	// FIXME: this should be triggered differently, by player choice
	if((tcp_sock >= 0 || web_sock >= 0) && countReadyClients() >= 2)
		{ 
      size_t i, count = 0;
      std::vector<size_t> players, nonplayers;
      for(i = 0; i < clients.size(); i++)
				{
					if((count < 2) &&
						 (clients[i]->isTCP() || clients[i]->isWebWait()) &&
						 (clients[i]->isStateSet(STATE_LOGGED_IN) &&
							!clients[i]->isStateSet(STATE_IN_GAME)))
						{
							players.push_back(i);
							count ++;
						}
					else
						{
							nonplayers.push_back(i);
						}
				}
			
      if(players.size() == 2)
        {
          // spawn a game
          pid_t pid = fork();
          if(pid > 0)
            { // parent
              DBGP_L(LOG_STREAM(std::cout) << "FORKED! Parent" << std::endl);
							
              size_t i;
              for(i = players.size(); i > 0; i--) {
                clients[players[i-1]]->forgetSocket();
                delete *(clients.begin() + players[i-1]);
                clients.erase(clients.begin() + players[i-1], clients.begin() + players[i-1] + 1);  // I know it's inefficent
              }
            }
          else if(pid == 0)
            { // child
              DBGP_L(LOG_STREAM(std::cout) << "FORKED! Child" << std::endl);
							
              size_t i;
              for(i = nonplayers.size(); i > 0; i--) {
                // I know it's inefficent
                clients[nonplayers[i-1]]->forgetSocket();
                delete *(clients.begin() + nonplayers[i-1]);
                clients.erase(clients.begin() + nonplayers[i-1], clients.begin() + nonplayers[i-1] + 1);
              }

              tcp_sock = -1;
              web_sock = -1;
              
              /*
               * Clients is an ordered vector.
               * We now have 2 clients left.
               * clients[0] will be forced to be player X
               * clients[1] will be forced to be player O
               *
               * We shuffle the vector. No preference is
               * given to first or second player to join game.
               */
              m_sent_you_are = false;
              std::random_shuffle(clients.begin(), clients.end());
              
              m_game = new UTTT;

							if(m_accounts)
								{
									m_game_id = m_accounts->startGame((*clients.begin())->getData(K_USERNAME),
																										(*(clients.begin()+1))->getData(K_USERNAME));
																				
								}
							DBGP_GAME(LOG_STREAM(std::cout)
												<< "New game[" << m_game_id << "] between " << (*clients.begin())->getData(K_USERNAME)
												<< " and " << (*(clients.begin()+1))->getData(K_USERNAME)
												<< std::endl);


              if(!sendGameUpdates(true))
                {
									DBGP_E(LOG_STREAM(std::cerr) << "sendGameUpdates failed." << std::endl);
									
                  return -1;
                }
            }
          else
            {
							DBGP_E(LOG_STREAM(std::cerr) << "Fork failed." << std::endl);

              return -1;
            }
        }
		}
	else if((tcp_sock >= 0 || web_sock >= 0) && countReadyClients() == 1 && timedout)
		{
			if(!spawnAI())
				{
					DBGP_E(LOG_STREAM(std::cerr) << "spawnAI() failed." << std::endl);
							
					return -1;
				}
		}
	else if(tcp_sock < 0 && web_sock < 0)
		{
			// check to see if one player has left, if so, close the other player's connection
			if(clients.size() == 1)
				{
					DBGP_L(LOG_STREAM(std::cout) << "1 game client, close it." << std::endl);
					if(!handleClientClosed(*(clients.begin()), 0))
						{
							DBGP_E(LOG_STREAM(std::cerr) << "handleClientClosed failed." << std::endl);
							
							return -1;
						}
				}
			// handling game, send game updates to clients.
			if(!sendGameUpdates(false))
				{
					DBGP_E(LOG_STREAM(std::cerr) << "sendGameUpdates failed." << std::endl);
					
					return -1;
				}
			
			// handling game over
			if(m_game && m_game->getWinner() != PLAYER_N && clients.size() > 1)
				{
					DBGP_GAME(LOG_STREAM(std::cout)
										<< "Game[" << m_game_id << "] over between " << (*clients.begin())->getData(K_USERNAME)
										<< " and " << (*(clients.begin()+1))->getData(K_USERNAME)
										<< " winner: " << m_game->getWinner()
										<< std::endl);
					
					if(m_accounts)
						{
							m_accounts->endGame((*clients.begin())->getData(K_USERNAME),
																	(*(clients.begin()+1))->getData(K_USERNAME),
																	m_game->getWinner(),
																	m_game_id);
							
							DBGP_GAME(LOG_STREAM(std::cout)
												<< "Force logout " << (*clients.begin())->getData(K_USERNAME)
												<< std::endl);
							
							m_accounts->logout((*clients.begin())->getData(K_USERNAME));
							
							DBGP_GAME(LOG_STREAM(std::cout)
												<< "Force logout " << (*(clients.begin()+1))->getData(K_USERNAME)
												<< std::endl);
							
							m_accounts->logout((*(clients.begin()+1))->getData(K_USERNAME));
						}

					int ii;
					for(ii = 1; ii >= 0; ii--)
						{
							DBGP_SOCKET(LOG_STREAM(std::cerr) << "Closing client " << ii << std::endl);
							
							(*(clients.begin()+ii))->closeSocket();
							deleteClient(ii);
						}
				}
		}
	
	return 0;
}


bool UTTTServer::sendGameUpdates(bool send_everything)
{
	std::vector<const Messaging::IMsg *> msgs;

	if(send_everything || !m_sent_you_are)
		{
			size_t i;
			for(i = 0; i < 2 && i < clients.size(); i++) {
				if(!sendYouAre(clients[i], i)) {
          DBGP_E(LOG_STREAM(std::cerr) << "sendYouAre failed on client " << i << std::endl);
				}
				if(!sendOpponentName(clients[i], i, 1-i)) {
          DBGP_E(LOG_STREAM(std::cerr) << "sendOpponentName failed on client " << i << std::endl);
				}
			}
			m_sent_you_are = true;
		}

	if(send_everything || m_game->hasPlayerChanged() || m_game->hasNextBoardChanged())
		{
			/*
			 * Send NextTurn message
			 */
			int next_player = m_game->getPlayer();
			int next_board = m_game->getNextBoard();
			const Messaging::NextTurnMsg* rmsg = new Messaging::NextTurnMsg(player_symbol(next_player), next_board);
			msgs.push_back(rmsg);
		}

	if(send_everything || m_game->hasWinnerChanged())
		{
			/*
			 * Send WinState message
			 */
			int winner = m_game->getWinner();
			const Messaging::WinStateMsg* const rmsg = new Messaging::WinStateMsg(player_symbol(winner));
			msgs.push_back(rmsg);
		}

	{
		/*
		 * Send BoardState messages
		 */
		int i,j;
		for(i = 0; i <= UTTT_NUM_POSITIONS; i++)
			{
				if(send_everything || m_game->hasOwnerChanged(i))
					{
						int winner = m_game->getOwner(i);
						const Messaging::BoardStateMsg* const rmsg = new Messaging::BoardStateMsg(player_symbol(winner), i);
						msgs.push_back(rmsg);
					}
				for(j = 0; j <= TTT_NUM_POSITIONS; j++)
					{
						if(send_everything || m_game->hasPositionChanged(i, j))
							{
								int player = m_game->getPosition(i, j);
								const Messaging::MarkerMsg* const rmsg = new Messaging::MarkerMsg(player_symbol(player), i, j);
								msgs.push_back(rmsg);
							}
					}
			}
	}

	if(msgs.size() > 0)
		{
			const Messaging::UpdateFinishedMsg* const rmsg = new Messaging::UpdateFinishedMsg();
			msgs.push_back(rmsg);
		}
	
	size_t i;
	for(i = 0; i < msgs.size(); i++)
		{
			const Messaging::IMsg* const rmsg = msgs[i];
			std::vector<ConnectionClient *>::iterator it;
			for(it = clients.begin(); it != clients.end(); it++)
				{
					if(!sendMessage(*it, rmsg))
						{
							DBGP_E(LOG_STREAM(std::cerr) << "sendMessage() failed" << std::endl);
							
							return false;
						}
				}
			delete rmsg;
		}
	
	msgs.clear();
	m_game->resetChangeArrays();
	return true;
}

bool UTTTServer::sendYouAre(ConnectionClient *client, int i)
{
	const Messaging::YouAreMsg* rmsg = new Messaging::YouAreMsg(player_symbol(player_from_index(i)),
																															client->getData(K_USERNAME));
	if(!sendMessage(client, rmsg))
		{
			DBGP_E(LOG_STREAM(std::cerr) << "sendMessage failed" << std::endl);
			
			return false;
		}

	delete rmsg;
	return true;
}

bool UTTTServer::sendOpponentName(ConnectionClient *client, int i, int opponent_i)
{
	const Messaging::OpponentNameMsg* rmsg = new Messaging::OpponentNameMsg(clients[opponent_i]->getData(K_USERNAME));
	if(!sendMessage(client, rmsg))
		{
			DBGP_E(LOG_STREAM(std::cerr) << "sendMessage failed" << std::endl);
			
			return false;
		}
	
	delete rmsg;
	return true;
}

bool UTTTServer::sendTurnFailed(ConnectionClient *client, int i, const char *message)
{
	const Messaging::TurnFailedMsg* rmsg = new Messaging::TurnFailedMsg(player_symbol(player_from_index(i)), message);
	if(!sendMessage(client, rmsg))
		{
			DBGP_E(LOG_STREAM(std::cerr) << "sendMessage faild" << std::endl);
			
			return false;
		}
	
	delete rmsg;
	return true;
}

bool UTTTServer::sendMessage(ConnectionClient *client, const Messaging::IMsg* const rmsg)
{
	Serialization::Serializer ser;

	std::string data, err;
	if ((err = ser.Serialize(rmsg, data)) != "")
		{
			DBGP_E(LOG_STREAM(std::cerr) << "Serialize Failure => " << err << std::endl);
			return false;
		}
	
	std::vector<char> msg(data.size());
	std::copy(data.begin(), data.end(), msg.begin());
	DBGP_SOCKET(LOG_STREAM(std::cerr) << *client << " :: " << data << std::endl);
	
	int n = client->write(msg);
	if(n < 0)
		{
			DBGP_E(LOG_STREAM(std::cerr) << "Message write failure." << std::endl);
			return false;
		}
	
	return true;
}

bool UTTTServer::handleClientClosed(ConnectionClient *client, int i)
{
	if(m_accounts)
		{
			if(clients.size() > 1 && m_game)
				{
					DBGP_GAME(LOG_STREAM(std::cout)
										<< "Game[" << m_game_id << "] over between " << (*clients.begin())->getData(K_USERNAME)
										<< " and " << (*(clients.begin()+1))->getData(K_USERNAME)
										<< " winner: " << m_game->getWinner()
										<< std::endl);
					
					m_accounts->endGame((*clients.begin())->getData(K_USERNAME),
															(*(clients.begin()+1))->getData(K_USERNAME),
															m_game->getWinner(),
															m_game_id);
				}
			
			DBGP_GAME(LOG_STREAM(std::cout)
								<< "Force logout " << client->getData(K_USERNAME)
								<< std::endl);
			m_accounts->logout(client->getData(K_USERNAME));
		}

	client->closeSocket();
	this->deleteClient(i);
	return true;
}

bool UTTTServer::spawnAI()
{
	DBGP_L(LOG_STREAM(std::cerr) << "Spawning AI" << std::endl);
	
	// spawn an ai
	pid_t pid = fork();

	if(pid > 0)
		{
			DBGP_L(LOG_STREAM(std::cerr) << "Parent of spawn done." << std::endl);
		}
	else
		{
			DBGP_L(LOG_STREAM(std::cerr) << "Child of spawn about to execv." << std::endl);
			
			const char *path = "../cpp-uttt-client";
			char * const argv[] = { (char *)"cpp-uttt-client", (char *)"-a", (char *)"-U", (char *)"barney", (char *)"-P", (char *)"bam123", (char *)0 };
			if(execv(path, argv) < 0)
				{
					DBGP_E(LOG_STREAM(std::cerr) << "Execv failed." << std::endl);

					exit(1);
				}
		}
	return true;
}

int UTTTServer::countReadyClients() const
{
	int count = 0;
	size_t i;
	for(i = 0; i < clients.size(); i++)
		{
			if((clients[i]->isTCP() || clients[i]->isWebWait()) &&
				 clients[i]->isStateSet(STATE_LOGGED_IN))
				{
					count ++;
				}
		}

	return count;
}
