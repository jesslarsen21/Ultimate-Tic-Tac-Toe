#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class PingMsg : public IMsg {
    public:
        static const std::string GTypeName;

        PingMsg();
        PingMsg(const PingMsg& msg);
        virtual ~PingMsg();

        std::string GetTypeName() const;

        friend std::ostream& operator<<(std::ostream&, const PingMsg&);
    };
}
