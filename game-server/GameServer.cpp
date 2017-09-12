#include "GameServer.h"
#include <vector>

GameServer::GameServer()
{
}

GameServer::~GameServer()
{
}

bool GameServer::mainLoop()
{
	Message msg;
	while(!game.isGameOver())
		{
			waitForMessage(msg);
			processMessage(msg);
			sendGameUpdates();
		}
	return true;
}

bool GameServer::waitForMessage(Message &msg)
{
	return true;
}

bool GameServer::processMessage(Message &msg)
{
	return true;
}

bool GameServer::sendGameUpdates()
{
	std::vector<Message> msgs;
	// FIXME: set msgs from game
	std::vector<Message>::iterator mit;
	std::vector<ClientConnection>::iterator cit;
	for(mit = msgs.begin(); mit != msgs.end(); mit++)
	{
		for(cit = this->clients.begin();
			cit != this->clients.end();
			cit++)
			{
				cit->sendMessage(*mit);
			}
	}
	return true;
}

