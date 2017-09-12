#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class NextTurnMsg : public IMsg {
    protected:
        char player;
        int nextBoard;

    public:
        static const std::string GTypeName;

        NextTurnMsg(const char player, const int nextBoard);
        NextTurnMsg(const NextTurnMsg& msg);
        virtual ~NextTurnMsg();

        std::string GetTypeName() const;
        char GetPlayer() const;
        int GetNextBoard() const;

        friend std::ostream& operator<<(std::ostream&, const NextTurnMsg&);
    };
}
