#include "TurnFailedMsg.h"

namespace Messaging {
    const std::string TurnFailedMsg::GTypeName = "turnfailed";

    TurnFailedMsg::TurnFailedMsg(const char player, std::string message)
        : player(player), message(message) { }

    TurnFailedMsg::TurnFailedMsg(const TurnFailedMsg& msg) 
        : player(msg.player), message(msg.message) { }

    TurnFailedMsg::~TurnFailedMsg() { }

    std::string TurnFailedMsg::GetTypeName() const { 
        return GTypeName;
    }

    char TurnFailedMsg::GetPlayer() const {
        return player; 
    }

    std::string TurnFailedMsg::GetMessage() const {
        return message;
    }

	std::ostream& operator<<(std::ostream& os, const Messaging::TurnFailedMsg& msg) {
    os  << msg.GetTypeName() << ' '
        << msg.GetPlayer() << ' '
        << msg.GetMessage();
		
    return os;
	}
	
}

