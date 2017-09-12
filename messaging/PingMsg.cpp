#include "PingMsg.h"

namespace Messaging {
    const std::string PingMsg::GTypeName = "ping";

    PingMsg::PingMsg() { }
    PingMsg::PingMsg(const PingMsg& msg) { }
    PingMsg::~PingMsg() { }

    std::string PingMsg::GetTypeName() const {
        return GTypeName;
    }

	std::ostream& operator<<(std::ostream& os, const Messaging::PingMsg& msg) {
    os << msg.GetTypeName();
		
    return os;
	}
}
 
