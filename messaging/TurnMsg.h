#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class TurnMsg : public IMsg {
    protected:
        char player;
        int board;
        int position;  
        
    public:
        static const std::string GTypeName;

        TurnMsg(const char player, const int board, const int position);
        TurnMsg(const TurnMsg& turn);
        virtual ~TurnMsg();

        char GetPlayer() const;
        int GetBoard() const;
        int GetPosition() const;
        std::string GetTypeName() const;

        friend std::ostream& operator<<(std::ostream&, const TurnMsg&);
    };
}
