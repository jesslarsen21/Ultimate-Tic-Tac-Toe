#include "SignUpReplyMsg.h"

namespace Messaging {
    const std::string SignUpReplyMsg::GTypeName = "signupreply";

    SignUpReplyMsg::SignUpReplyMsg(
            const bool usernameValid,
            const bool emailValid,
            const PasswordState passwordState)
        : usernameValid(usernameValid),
          emailValid(emailValid),
          passwordState(passwordState) { }

    SignUpReplyMsg::SignUpReplyMsg(const SignUpReplyMsg& msg)
        : usernameValid(msg.usernameValid),
          emailValid(msg.emailValid),
          passwordState(msg.passwordState) { }

    SignUpReplyMsg::~SignUpReplyMsg() { }

    std::string SignUpReplyMsg::GetTypeName() const {
        return GTypeName;
    }

    bool SignUpReplyMsg::IsUsernameValid() const {
        return usernameValid;
    }

    bool SignUpReplyMsg::IsEmailValid() const {
        return emailValid;
    }

    SignUpReplyMsg::PasswordState SignUpReplyMsg::GetPasswordState() const {
        return passwordState; 
    }

	std::ostream& operator<<(std::ostream& os, const Messaging::SignUpReplyMsg& msg) {
    const char* passState;
		
    switch (msg.GetPasswordState()) {
    case Messaging::SignUpReplyMsg::Invalid:
			passState = "invalid";
			break;
    case Messaging::SignUpReplyMsg::Valid:
			passState = "valid";
			break;
    case Messaging::SignUpReplyMsg::Unmatched:
			passState = "unmatched";
			break;
    default:
			passState = "UNDEFINED";
			break;
    };
		
    os  << msg.GetTypeName() << ' '
        << ( msg.IsUsernameValid() ? "valid" : "invalid" ) << ' '
        << ( msg.IsEmailValid() ? "valid" : "invalid" ) << ' '
        << passState;
		
    return os;
	}
	
}

