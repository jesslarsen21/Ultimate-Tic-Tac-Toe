#include "UpdateFinishedMsg.h"

namespace Messaging {
    const std::string UpdateFinishedMsg::GTypeName = "updatefinished";

    UpdateFinishedMsg::UpdateFinishedMsg() { }
    UpdateFinishedMsg::UpdateFinishedMsg(const UpdateFinishedMsg& msg) { }
    UpdateFinishedMsg::~UpdateFinishedMsg() { }

    std::string UpdateFinishedMsg::GetTypeName() const {
        return GTypeName;
    }
	
	std::ostream& operator<<(std::ostream& os, const Messaging::UpdateFinishedMsg& msg) { 
    os << msg.GetTypeName();
		
    return os;
	}
}

