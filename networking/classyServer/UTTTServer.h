#ifndef _UTTTSERVER_H_
#define _UTTTSERVER_H_

#include "MessagingServer.h"
#include "UTTT.h"
#include "accounts.h"
#include "Messaging.h"

class UTTTServer: public MessagingServer
{
public:
  UTTTServer();
  virtual ~UTTTServer();
	virtual int handleClient(ConnectionClient *client, int i);
	virtual int heartbeat(bool timedout);
	virtual bool sendGameUpdates(bool send_everything);
  virtual bool sendMessage(ConnectionClient *client, const Messaging::IMsg* const rmsg);
	virtual bool sendYouAre(ConnectionClient *client, int i);
	virtual bool sendOpponentName(ConnectionClient *client, int i, int opponent_i);
  virtual bool sendTurnFailed(ConnectionClient *client, int i, const char *message);
	virtual bool handleClientClosed(ConnectionClient *client, int i);
	virtual bool spawnAI();
	virtual int countReadyClients() const;
  
protected:
  enum {
    STATE_LOGGED_IN = 0x4000,
    STATE_IN_GAME   = 0x2000
  };

  DB   *m_accounts;
  UTTT *m_game;
	bool  m_sent_you_are;
	int   m_game_id;
  
private:
  
};

#endif /* _UTTTSERVER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
