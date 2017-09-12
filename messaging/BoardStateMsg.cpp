#include "BoardStateMsg.h"

namespace Messaging {
    const std::string BoardStateMsg::GTypeName = "boardstate";

    BoardStateMsg::BoardStateMsg(const char player, const int board)
        : player(player), board(board) { }

    BoardStateMsg::BoardStateMsg(const BoardStateMsg& msg) 
        : player(msg.player), board(msg.board) { }

    BoardStateMsg::~BoardStateMsg() { }

    std::string BoardStateMsg::GetTypeName() const { 
        return GTypeName;
    }

    char BoardStateMsg::GetPlayer() const {
        return player; 
    }

    int BoardStateMsg::GetBoard() const { 
        return board;
    }
	std::ostream& operator<<(std::ostream& os, const Messaging::BoardStateMsg& msg) {
    os  << msg.GetTypeName() << ' '
        << msg.GetPlayer() << ' '
        << msg.GetBoard();
		
    return os;
	}
}

