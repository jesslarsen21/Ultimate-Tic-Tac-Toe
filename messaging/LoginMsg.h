#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class LoginMsg : public IMsg {
    private:
        const std::string email;
        const std::string password;

    public:
        static const std::string GTypeName;

        LoginMsg(const std::string& email, const std::string& password);
        LoginMsg(const LoginMsg& msg);
        virtual ~LoginMsg();

        std::string GetTypeName() const;
        std::string GetEmail() const;
        std::string GetPassword() const;

        friend std::ostream& operator<<(std::ostream&, const LoginMsg&);
    };
}
