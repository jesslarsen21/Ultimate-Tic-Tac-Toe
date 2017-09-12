#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class ErrorMsg : public IMsg {
    protected:
        // (timestamp) L(severity) (sender) (what) (reason)
        // 3rd Apr 10:00 | L0 | Server | Login failed | Conn failed
        std::string when;
        int severity;
        std::string sender;
        std::string what;
        std::string reason;
        const ErrorMsg* nextError;

    public:
        static const std::string GTypeName;
        
        ErrorMsg(const std::string& when,
                 const int severity,
                 const std::string& sender,
                 const std::string& what,
                 const std::string& reason,
                 const ErrorMsg* const nextError);

        ErrorMsg(const ErrorMsg& msg);
        virtual ~ErrorMsg();

        std::string GetTypeName() const;
        std::string GetWhen() const;
        int GetSeverity() const;

        std::string GetSender() const;
        std::string GetWhat() const;
        std::string GetReason() const;
        const ErrorMsg* const GetNextError() const;

        // <summary>
        // Returns this error's information as a formatted string.
        // - 10 Apr 2015 10:00pm | L0 | Server:auth0 | Login failed | Conn failed 
        // </summary>
        std::string GetMessage(const std::string& separator) const;

        // <summary>
        // Returns the entire stack trace for this error in one big string. 
        // Each successive error will be separated by a newline character.
        // - Example: 
        //   10 Apr 2015 10:00pm | L0 | Server:auth0 | Login failed | Conn failed \n
        //   10 Apr 2015 10:01pm | L0 | Client:bob | Login failed | Exception thrown 
        // </summary> 
        std::string GetStackTrace(const std::string& separator) const;

        friend std::ostream& operator<<(std::ostream& os, const ErrorMsg& msg);
    };
}
