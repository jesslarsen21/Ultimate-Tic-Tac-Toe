#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class WinStateMsg : public IMsg {
    protected:
        char player;

    public:
        static const std::string GTypeName;

        WinStateMsg(const char player);
        WinStateMsg(const WinStateMsg& msg);
        virtual ~WinStateMsg();

        std::string GetTypeName() const;
        char GetPlayer() const;

        friend std::ostream& operator<<(std::ostream&, const WinStateMsg& msg);
    };
}
