#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class SignUpReplyMsg : public IMsg {
    public:
        enum PasswordState {
            Invalid,
            Valid,
            Unmatched
        };

    private:
        bool usernameValid;
        bool emailValid;
        PasswordState passwordState;

    public:
        static const std::string GTypeName;

        SignUpReplyMsg(
                const bool usernameValid,
                const bool emailValid,
                const PasswordState passwordState
                );
        SignUpReplyMsg(const SignUpReplyMsg& msg);
        virtual ~SignUpReplyMsg();

        std::string GetTypeName() const;
        bool IsUsernameValid() const;
        bool IsEmailValid() const;
        PasswordState GetPasswordState() const;

        friend std::ostream& operator<<(std::ostream&, const SignUpReplyMsg&);
    };
}
