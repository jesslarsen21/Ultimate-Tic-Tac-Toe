#include "HttpSegments.h"
#include "WebSocketUtils.h"
#include "WebSocketValidator.h"
#include <iostream>

namespace Net {
	namespace WebSocket {
		WebSocketValidator::WebSocketValidator(const std::string url)
			: rules(url) {
		}
			
		/// <summary>
		/// Attempts to validate the given data to a WebSocket handshake request.
		/// </summary>
		/// <param name="data"> The handshake to validate. </param>
		/// <param name="accept"> The destination for the resulting accept string, if valid. </param>
		/// <returns>
		/// Returns an error string if the handshake was invalid.
		/// </returns>
		std::string WebSocketValidator::Validate(const std::vector<char> &data, std::vector<char>& accept) const {
			static const std::string httpResp = "HTTP/1.1 404\r\n\r\n";
			std::string err;
			Http::HttpSegments httpReq;
			
			err = Http::HttpSegments::Parse(data, httpReq);
			if (err != "") {
				accept.resize(httpResp.size());
				std::copy(httpResp.begin(), httpResp.end(), accept.begin());
				return err;
			}
			
			if (!rules.IsWebSocketUpgrade(httpReq.HeaderSegments)) {
				accept.resize(httpResp.size());
				std::copy(httpResp.begin(), httpResp.end(), accept.begin());
				return "Invalid WebSocket upgrade.";
			}
			
			std::string socketKey;
			const std::string key = "Sec-WebSocket-Key: ";
			if (!rules.GetKey(key, httpReq.HeaderSegments, socketKey)) {
				accept.resize(httpResp.size());
				std::copy(httpResp.begin(), httpResp.end(), accept.begin());
				return "Unable to find required Sec-WebSocket-Key: $value from header.";
			}
			
			const std::string acceptCode = rules.CalcWSAccept6455(socketKey);
			const std::string accept_string = rules.CreateAcceptString6455(acceptCode);
			accept.resize(accept_string.size());
			std::copy(accept_string.begin(), accept_string.end(), accept.begin());
			
			return "";
		}
	}
}
