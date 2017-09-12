#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class SignUpMsg : public IMsg {
    private:
        const std::string username;
        const std::string email;
        const std::string password;
        const std::string passwordConfirm;

    public:
        static const std::string GTypeName;

        SignUpMsg(
                const std::string& username, 
                const std::string& email, 
                const std::string& password,
                const std::string& passwordConfirm
                );
        SignUpMsg(const SignUpMsg& msg);
        virtual ~SignUpMsg();

        std::string GetTypeName() const;
        std::string GetUsername() const;
        std::string GetEmail() const;
        std::string GetPassword() const;
        std::string GetPasswordConfirm() const;
        
        friend std::ostream& operator<<(std::ostream&, const SignUpMsg& msg);
    };
}
