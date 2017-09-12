#ifndef _MESSAGINGCLIENT_H_
#define _MESSAGINGCLIENT_H_

#include "Client.h"

class MessagingClient : public Client
{
public:
  MessagingClient();
  virtual ~MessagingClient();
  virtual int handleServer();
	virtual int handleStdin();
  
protected:
  virtual int buildResponse(const std::vector<char> &msg, std::vector<char> &response);
  
private:
  
};

#endif /* _MESSAGINGCLIENT_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
