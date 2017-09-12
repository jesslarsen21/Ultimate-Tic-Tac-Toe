#ifndef _CLIENTCONNECTION_H_
#define _CLIENTCONNECTION_H_

#include "Message.h"

// This class actually needs to be created somewhere
class ClientConnection
{
public:
	bool sendMessage(const Message &msg);
};

#endif /* _CLIENTCONNECTION_H_ */
/* Local Variables: */
/* mode:c++         */
/* End:             */
