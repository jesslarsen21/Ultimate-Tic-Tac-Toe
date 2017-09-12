#include "LoginReplyMsg.h"

namespace Messaging {
    const std::string LoginReplyMsg::GTypeName = "loginreply";

    LoginReplyMsg::LoginReplyMsg(const bool valid, const std::string& token)
        : valid(valid), token(token) { }

    LoginReplyMsg::LoginReplyMsg(const LoginReplyMsg& msg)
        : valid(msg.valid), token(msg.token) { }

    LoginReplyMsg::~LoginReplyMsg() { }

    std::string LoginReplyMsg::GetTypeName() const {
        return GTypeName;
    }

    bool LoginReplyMsg::IsValid() const {
        return valid; 
    }

    std::string LoginReplyMsg::GetToken() const {
        return token; 
    }
	
	std::ostream& operator<<(std::ostream& os, const Messaging::LoginReplyMsg& msg) {
    os  << msg.GetTypeName() << ' '
        << ( msg.IsValid() ? "valid" : "invalid" ) << ' '
        << msg.GetToken();
		
    return os;
	}
}

