#pragma once

#include <string>
#include "WebSocketValidator.h"

namespace Net {
	namespace WebSocket {
		class WebSocketServer : public SocketServer {
		private:
			const WebSocketValidator validator;
		
		public:
			WebSocketServer(const std::string url)
				: validator(url) {
			}
			
			bool handshaking(const std::string data, std::string& accept) const {
				
			}
		};
	}
}
