#pragma once

#include <string>
#include "IMsg.h"

namespace Serialization {
	class Serializer {
	private:
		static const std::string GHeaderKey;  
		static const std::string GSeparator;
		static const std::string GEndMsgKey;
		
		static const std::string ErrLeadingTypeCode;

    public:

        std::string Serialize(const Messaging::IMsg* const msg, std::string& result) const; 

        std::string Deserialize(const std::string data, Messaging::IMsg*& result) const;
    };
}
