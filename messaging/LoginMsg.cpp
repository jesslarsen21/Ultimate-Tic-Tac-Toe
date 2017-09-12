#include "LoginMsg.h"

namespace Messaging {
    const std::string LoginMsg::GTypeName = "login";

    LoginMsg::LoginMsg(const std::string& email, const std::string& password)
        : email(email), password(password) { }

    LoginMsg::LoginMsg(const LoginMsg& msg)
        : email(msg.email), password(msg.password) { }

    LoginMsg::~LoginMsg() { }

    std::string LoginMsg::GetTypeName() const {
        return GTypeName;
    }

    std::string LoginMsg::GetEmail() const { 
        return email;
    }

    std::string LoginMsg::GetPassword() const {
        return password;
    }
	
	std::ostream& operator<<(std::ostream& os, const Messaging::LoginMsg& msg) {
    os  << msg.GetTypeName() << ' '
        << msg.GetEmail() << ' '
        << msg.GetPassword();
		
    return os;
	}
}

