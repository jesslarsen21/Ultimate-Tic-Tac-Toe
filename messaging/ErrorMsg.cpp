#include <sstream>
#include "ErrorMsg.h"

namespace Messaging {
    const std::string ErrorMsg::GTypeName = "error";

    ErrorMsg::ErrorMsg(
            const std::string& when,
            const int severity,
            const std::string& sender,
            const std::string& what,
            const std::string& reason,
            const ErrorMsg* const nextError
        ) : when(when),
            severity(severity),
            sender(sender),
            what(what),
            reason(reason),
            nextError(nextError) {
    }
            
    ErrorMsg::ErrorMsg(const ErrorMsg& msg) 
        :   when(msg.when),
            severity(msg.severity),
            sender(msg.sender),
            what(msg.what),
            reason(msg.reason),
            nextError(msg.nextError) {
    }

    ErrorMsg::~ErrorMsg() { 
        if (this->nextError) {
            delete this->nextError;
        }
        this->nextError = NULL;
    }

    std::string ErrorMsg::GetTypeName() const { 
        return GTypeName;
    }

    std::string ErrorMsg::GetWhen() const {
        return when;
    }

    int ErrorMsg::GetSeverity() const {
        return severity; 
    }

    std::string ErrorMsg::GetSender() const {
        return sender; 
    }

    std::string ErrorMsg::GetWhat() const {
        return what;
    }

    std::string ErrorMsg::GetReason() const {
        return reason;
    }

    const ErrorMsg* const ErrorMsg::GetNextError() const {
        return nextError;
    }

    std::string ErrorMsg::GetMessage(const std::string& separator) const {
        std::ostringstream os;

        os  << when << separator
            << 'L' << severity << separator
            << sender << separator
            << what << separator
            << reason;

        return os.str();
    }

    std::string ErrorMsg::GetStackTrace(const std::string& separator) const {
        std::ostringstream os;

        os << this->GetMessage(separator);
        if (this->nextError) {
           os << std::endl;
           os << nextError->GetStackTrace(separator);
        }

        return os.str();
    }
	std::ostream& operator<<(std::ostream& os, const Messaging::ErrorMsg& msg) {
    os  << msg.GetTypeName() << ' '
        << msg.GetMessage(" ");
		
    return os;
	}
}

