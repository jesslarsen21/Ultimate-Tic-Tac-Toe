#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class LoginReplyMsg : public IMsg {
    private:
        const bool valid;
        const std::string token;

    public:
        static const std::string GTypeName;

        LoginReplyMsg(const bool valid, const std::string& token);
        LoginReplyMsg(const LoginReplyMsg& msg);
        virtual ~LoginReplyMsg();

        std::string GetTypeName() const;
        bool IsValid() const;
        std::string GetToken() const;

        friend std::ostream& operator<<(std::ostream&, const LoginReplyMsg&);
    };
}
