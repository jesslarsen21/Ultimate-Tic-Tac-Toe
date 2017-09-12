#pragma once
#include "WebSocketUtils.h"
#include <vector>
#include <string>
namespace Net {
	namespace WebSocket {
		class WebSocketValidator {
		public:
			ParseRules rules;
			WebSocketValidator(const std::string url);
			std::string Validate(const std::vector<char> &data, std::vector<char>& accept) const;
		};
	}
}
