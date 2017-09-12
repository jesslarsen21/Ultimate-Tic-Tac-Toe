#include "SignUpMsg.h"

namespace Messaging {
    const std::string SignUpMsg::GTypeName = "signup";

    SignUpMsg::SignUpMsg(
            const std::string& username,
            const std::string& email, 
            const std::string& password,
            const std::string& passwordConfirm)
        : username(username), email(email), 
          password(password), passwordConfirm(passwordConfirm) { }

    SignUpMsg::SignUpMsg(const SignUpMsg& msg)
        : username(msg.username), email(msg.email), 
          password(msg.password), passwordConfirm(msg.passwordConfirm) { }

    SignUpMsg::~SignUpMsg() { }

    std::string SignUpMsg::GetTypeName() const {
        return GTypeName;
    }

    std::string SignUpMsg::GetUsername() const {
        return username;
    }

    std::string SignUpMsg::GetEmail() const { 
        return email;
    }

    std::string SignUpMsg::GetPassword() const {
        return password;
    }

    std::string SignUpMsg::GetPasswordConfirm() const {
        return passwordConfirm;
    }

	std::ostream& operator<<(std::ostream& os, const Messaging::SignUpMsg& msg) {
    os  << msg.GetTypeName() << ' '
        << msg.GetUsername() << ' '
        << msg.GetEmail() << ' '
        << msg.GetPassword() << ' '
        << msg.GetPasswordConfirm();
		
    return os;
	}
}
