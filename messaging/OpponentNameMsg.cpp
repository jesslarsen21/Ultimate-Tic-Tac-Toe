#include "OpponentNameMsg.h"

namespace Messaging {
    const std::string OpponentNameMsg::GTypeName = "opponentname";

    OpponentNameMsg::OpponentNameMsg(const std::string& username)
        : username(username) { }

    OpponentNameMsg::OpponentNameMsg(const OpponentNameMsg& msg) 
        : username(msg.username) { }

    OpponentNameMsg::~OpponentNameMsg() { }

    std::string OpponentNameMsg::GetTypeName() const { 
        return GTypeName;
    }

    std::string OpponentNameMsg::GetUsername() const { 
       return username; 
    }
	
	std::ostream& operator<<(std::ostream& os, const Messaging::OpponentNameMsg& msg) {
    os  << msg.GetTypeName() << ' '
        << msg.GetUsername();
		
    return os;
	}
}

