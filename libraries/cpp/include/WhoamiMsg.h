#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class WhoamiMsg : public IMsg {
    public:
        static const std::string GTypeName;

        WhoamiMsg();
        WhoamiMsg(const WhoamiMsg& msg);
        virtual ~WhoamiMsg();

        std::string GetTypeName() const;

        friend std::ostream& operator<<(std::ostream&, const WhoamiMsg&);
    };
}
