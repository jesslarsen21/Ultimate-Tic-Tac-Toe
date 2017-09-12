#include "NextTurnMsg.h"

namespace Messaging {
    const std::string NextTurnMsg::GTypeName = "nextturn";

    NextTurnMsg::NextTurnMsg(const char player, const int nextBoard)
        : player(player), nextBoard(nextBoard) { }

    NextTurnMsg::NextTurnMsg(const NextTurnMsg& msg) 
        : player(msg.player), nextBoard(msg.nextBoard) { }

    NextTurnMsg::~NextTurnMsg() { }

    std::string NextTurnMsg::GetTypeName() const { 
        return GTypeName;
    }

    char NextTurnMsg::GetPlayer() const {
        return player; 
    }

    int NextTurnMsg::GetNextBoard() const { 
        return nextBoard;
    }
	
	std::ostream& operator<<(std::ostream& os, const Messaging::NextTurnMsg& msg) {
    os  << msg.GetTypeName() << ' '
        << msg.GetPlayer() << ' '
        << msg.GetNextBoard();
		
    return os;
	}
}

