#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class UpdateMsg : public IMsg {
    public:
        static const std::string GTypeName;

        UpdateMsg();
        UpdateMsg(const UpdateMsg& msg);
        virtual ~UpdateMsg();

        std::string GetTypeName() const;

        friend std::ostream& operator<<(std::ostream&, const UpdateMsg&);
    };
}
