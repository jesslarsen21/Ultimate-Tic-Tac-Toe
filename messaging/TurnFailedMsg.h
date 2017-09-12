#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class TurnFailedMsg : public IMsg {
    protected:
        char player;
        std::string message;

    public:
        static const std::string GTypeName;

        TurnFailedMsg(const char player, std::string message);
        TurnFailedMsg(const TurnFailedMsg& msg);
        virtual ~TurnFailedMsg();

        std::string GetTypeName() const;
        char GetPlayer() const;
        std::string GetMessage() const;

        friend std::ostream& operator<<(std::ostream&, const TurnFailedMsg&);
    };
}
