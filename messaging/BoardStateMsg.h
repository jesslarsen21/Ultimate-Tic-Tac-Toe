#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class BoardStateMsg : public IMsg {
    protected:
        char player;
        int board;

    public:
        static const std::string GTypeName;

        BoardStateMsg(const char player, const int board);
        BoardStateMsg(const BoardStateMsg& msg);
			  virtual ~BoardStateMsg();

        std::string GetTypeName() const;
        char GetPlayer() const;
        int GetBoard() const;

        friend std::ostream& operator<<(std::ostream& os, const BoardStateMsg& msg);
    };
}
