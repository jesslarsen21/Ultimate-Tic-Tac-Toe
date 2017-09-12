#ifndef _MESSAGINGSERVER_H_
#define _MESSAGINGSERVER_H_

#include "Server.h"

class MessagingServer: public Server
{
public:
  MessagingServer();
  virtual ~MessagingServer();
	virtual int handleClient(ConnectionClient *client, int i);
  
protected:
	virtual int buildResponse(ConnectionClient *client, int i, const std::vector<char> &msg,
														std::vector<char> &response);
  
private:
  
};

#endif /* _MESSAGINGSERVER_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
