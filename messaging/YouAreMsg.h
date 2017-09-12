#pragma once

#include <ostream>
#include <string>
#include "IMsg.h"

namespace Messaging {
    class YouAreMsg : public IMsg {
    protected:
        char player;
		std::string name;

    public:
        static const std::string GTypeName;

        YouAreMsg(const char player, const std::string name);
        YouAreMsg(const YouAreMsg& msg);
        virtual ~YouAreMsg();

        std::string GetTypeName() const;
        char GetPlayer() const;
		std::string GetName() const;

        friend std::ostream& operator<<(std::ostream&, const YouAreMsg&);
    };
}
