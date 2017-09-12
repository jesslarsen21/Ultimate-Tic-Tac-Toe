#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class OpponentNameMsg : public IMsg {
    protected:
        std::string username;

    public:
        static const std::string GTypeName;

        OpponentNameMsg(const std::string& username);
        OpponentNameMsg(const OpponentNameMsg& msg);
        virtual ~OpponentNameMsg();

        std::string GetTypeName() const;
        std::string GetUsername() const;

        friend std::ostream& operator<<(std::ostream&, const OpponentNameMsg);
    };
}

std::ostream& operator<<(std::ostream&, const Messaging::OpponentNameMsg&);
