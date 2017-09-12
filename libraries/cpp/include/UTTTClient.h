#ifndef _UTTTCLIENT_H_
#define _UTTTCLIENT_H_


#include "MessagingClient.h"
#include "Player.h"
#include "Messaging.h"
#include <string>

class UTTTClient: public MessagingClient
{
public:
  UTTTClient();
  UTTTClient(bool is_ai);
  virtual ~UTTTClient();
  virtual int handleServer();
  virtual int handleStdin();
  virtual bool sendMessage(const Messaging::IMsg* const rmsg);
  virtual bool sendTurnMessage(char player, int board, int position);
  virtual bool sendLoginMessage(const std::string &username, const std::string &password);
	virtual int heartbeat();
	virtual bool setCredentials(const std::string &username, const std::string &password);

protected:
	void initializePlayer();
	
  Player *m_player;
	bool m_is_ai;
	bool m_have_updates;
	bool m_logged_in;
	bool m_game_over;
	std::string m_opponent;
	std::string m_username, m_password;
private:
  
};

#endif /* _UTTTCLIENT_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
