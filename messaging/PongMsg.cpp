#include "PongMsg.h"

namespace Messaging {
    const std::string PongMsg::GTypeName = "pong";

    PongMsg::PongMsg() { }
    PongMsg::PongMsg(const PongMsg& msg) { }
    PongMsg::~PongMsg() { }

    std::string PongMsg::GetTypeName() const {
        return GTypeName;
    }
	
	std::ostream& operator<<(std::ostream& os, const Messaging::PongMsg& msg) { 
    os << msg.GetTypeName();
		
    return os;
	}
}

