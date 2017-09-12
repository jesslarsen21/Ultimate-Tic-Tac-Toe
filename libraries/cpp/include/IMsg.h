#pragma once

#include <string>

namespace Messaging {
    class IMsg {
    public:
			virtual ~IMsg() { /* empty */ };
			virtual std::string GetTypeName() const = 0;
    };
}
