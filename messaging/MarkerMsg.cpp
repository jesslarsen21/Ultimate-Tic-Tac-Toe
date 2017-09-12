#include "MarkerMsg.h"

namespace Messaging {
    const std::string MarkerMsg::GTypeName = "marker";

    MarkerMsg::MarkerMsg(const char player, const int board, const int position)
        : player(player), board(board), position(position) { }

    MarkerMsg::MarkerMsg(const MarkerMsg& msg) 
        : player(msg.player), board(msg.board), position(msg.position) { }

    MarkerMsg::~MarkerMsg() { }

    std::string MarkerMsg::GetTypeName() const { 
        return GTypeName;
    }

    char MarkerMsg::GetPlayer() const {
        return player; 
    }

    int MarkerMsg::GetBoard() const { 
        return board;
    }

    int MarkerMsg::GetPosition() const {
        return position;
    }
	
	std::ostream& operator<<(std::ostream& os, const Messaging::MarkerMsg& msg) {
    os  << msg.GetTypeName() << ' '
        << msg.GetPlayer() << ' '
        << msg.GetBoard() << ' '
        << msg.GetPosition();
		
    return os;
	}
}

