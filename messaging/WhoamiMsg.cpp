#include "WhoamiMsg.h"

namespace Messaging {
    const std::string WhoamiMsg::GTypeName = "whoami";

    WhoamiMsg::WhoamiMsg() { }
    WhoamiMsg::WhoamiMsg(const WhoamiMsg& msg) { }
    WhoamiMsg::~WhoamiMsg() { }

    std::string WhoamiMsg::GetTypeName() const {
        return GTypeName;
    }

	std::ostream& operator<<(std::ostream& os, const Messaging::WhoamiMsg& msg) {
    os  << msg.GetTypeName();
    return os;
	}
}
