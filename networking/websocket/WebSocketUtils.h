#pragma once
#include <string>
#include <vector>

namespace Net {
	namespace WebSocket {
	
		class ParseRules {
		private:
			const std::string Guid6455;
			const std::string url;
			const std::vector<std::string> requiredHeaders;
			
			bool EndsWith(const std::string str, const char c);
			bool StartsWith(const std::string str, const char c);
			std::string TrimLeading(const std::string str, const char c);
			std::vector<std::string> GenRequiredHeaders();
			static const std::string base64_chars;
			static std::string Base64Encode(unsigned char const* bytes_to_encode, unsigned int in_len);
			int	IsUTF8(uint8_t* s);
			
		public:
			ParseRules(const std::string url);
			bool IsWebSocketUpgrade(const std::vector<std::string> headers) const;
			static void DisplayMessageDigest(unsigned *message_digest);
			std::string CalcWSAccept6455 (const std::string secWebSocketKey) const;
			bool GetKey(const std::string key, const std::vector<std::string> arr, std::string& result) const;
			std::string CreateAcceptString6455(const std::string acceptCode) const;
			std::string MakeFinalTextFrame(const std::string payload) const;
			// FIXME the frame might not be a valid c-string.  need to do something better
			int GetTextFramePayload(const std::vector<char> &frame, std::string& payload) const;
		};
	}
}










