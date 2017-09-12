#include "YouAreMsg.h"

namespace Messaging {
    const std::string YouAreMsg::GTypeName = "youare";

    YouAreMsg::YouAreMsg(const char player, const std::string name)
        : player(player), name(name) { }

    YouAreMsg::YouAreMsg(const YouAreMsg& msg) 
			: player(msg.player), name(msg.name) { }

    YouAreMsg::~YouAreMsg() { }

    std::string YouAreMsg::GetTypeName() const { 
        return GTypeName;
    }

    char YouAreMsg::GetPlayer() const {
        return player; 
    }

	std::string YouAreMsg::GetName() const {
		return name;
	}

	std::ostream& operator<<(std::ostream& os, const Messaging::YouAreMsg& msg) { 
    os  << msg.GetTypeName() << ' ' 
        << msg.GetPlayer() << ' '
				<< msg.GetName();

    return os;
	}
}

