#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class PongMsg : public IMsg {
    public:
        static const std::string GTypeName;

        PongMsg();
        PongMsg(const PongMsg& msg);
        virtual ~PongMsg();

        std::string GetTypeName() const;

        friend std::ostream& operator<<(std::ostream&, const PongMsg&);
    };
}
