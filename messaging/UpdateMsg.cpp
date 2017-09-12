#include "UpdateMsg.h"

namespace Messaging {
    const std::string UpdateMsg::GTypeName = "update";

    UpdateMsg::UpdateMsg() { }
    UpdateMsg::UpdateMsg(const UpdateMsg& msg) { }
    UpdateMsg::~UpdateMsg() { }

    std::string UpdateMsg::GetTypeName() const {
        return GTypeName;
    }
	
	std::ostream& operator<<(std::ostream& os, const Messaging::UpdateMsg& msg) {
    os  << msg.GetTypeName();
    return os;
	}
}

