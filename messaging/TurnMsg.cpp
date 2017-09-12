#include "TurnMsg.h"

namespace Messaging {
    const std::string TurnMsg::GTypeName = "turn";
    
    TurnMsg::TurnMsg(const char player, const int board, const int position) 
        : player(player), board(board), position(position) { }

    TurnMsg::TurnMsg(const TurnMsg& msg)
        : player(msg.player), board(msg.board), position(msg.position) { }

    TurnMsg::~TurnMsg() { }

    char TurnMsg::GetPlayer() const { 
        return player;
    }

    int TurnMsg::GetBoard() const {
        return board;
    }

    int TurnMsg::GetPosition() const {
        return position;
    }

    std::string TurnMsg::GetTypeName() const {
        return GTypeName;
    }
	
	std::ostream& operator<<(std::ostream& os, const Messaging::TurnMsg& msg) {
    os  << msg.GetTypeName() << ' ' 
        << msg.GetPlayer() << ' '
        << msg.GetBoard() << ' '
        << msg.GetPosition();
		
    return os;
	}
}

