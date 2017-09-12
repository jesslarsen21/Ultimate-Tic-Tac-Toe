#include "WinStateMsg.h"

namespace Messaging {
    const std::string WinStateMsg::GTypeName = "winstate";

    WinStateMsg::WinStateMsg(const char player)
        : player(player) { }

    WinStateMsg::WinStateMsg(const WinStateMsg& msg) 
        : player(msg.player) { }

    WinStateMsg::~WinStateMsg() { }

    std::string WinStateMsg::GetTypeName() const { 
        return GTypeName;
    }

    char WinStateMsg::GetPlayer() const {
        return player; 
    }

	std::ostream& operator<<(std::ostream& os, const Messaging::WinStateMsg& msg) { 
    os  << msg.GetTypeName() << ' '
        << msg.GetPlayer() ;
		
    return os;
	}
	
}

