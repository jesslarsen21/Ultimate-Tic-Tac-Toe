#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class MarkerMsg : public IMsg {
    protected:
        char player;
        int board;
        int position;

    public:
        static const std::string GTypeName;

        MarkerMsg(const char player, const int board, const int position);
        MarkerMsg(const MarkerMsg& msg);
        virtual ~MarkerMsg();

        std::string GetTypeName() const;
        char GetPlayer() const;
        int GetBoard() const;
        int GetPosition() const;

        friend std::ostream& operator<<(std::ostream&, const MarkerMsg&);
    };
}
