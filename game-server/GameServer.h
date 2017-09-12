#ifndef _GAMESERVER_H_
#define _GAMESERVER_H_
#include <vector>
#include "UTTT.h"
#include "ClientConnection.h"
#include "Message.h"

class GameServer
{
public:
	GameServer();
	virtual ~GameServer();
	bool mainLoop();
	bool waitForMessage(Message &msg);
	bool processMessage(Message &msg);
	bool sendGameUpdates();
	
protected:
	UTTT game;
	std::vector<ClientConnection> clients;
	
private:
};

#endif /* _GAMESERVER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
