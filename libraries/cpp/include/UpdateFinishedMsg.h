#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class UpdateFinishedMsg : public IMsg {
    public:
        static const std::string GTypeName;

        UpdateFinishedMsg();
        UpdateFinishedMsg(const UpdateFinishedMsg& msg);
        virtual ~UpdateFinishedMsg();

        std::string GetTypeName() const;

        friend std::ostream& operator<<(std::ostream&, const UpdateFinishedMsg&);
    };
}
