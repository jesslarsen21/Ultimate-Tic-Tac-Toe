#include "WebSocketUtils.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <endian.h>
#include <cstdint>
#include <fstream>
#include "sha1.hpp"
#include "base64.hpp"
#include "utf8.hpp"


#define UTF8_ACCEPT 0
#define UTF8_REJECT 12

namespace Net {
	namespace WebSocket {
	
			
		static const uint8_t utf8d[] = {
		  // The first part of the table maps bytes to character classes that
		  // to reduce the size of the transition table and create bitmasks.
		   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		   1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
		   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		   8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		  10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,

		  // The second part is a transition table that maps a combination
		  // of a state of the automaton and a character class to a state.
		   0,12,24,36,60,96,84,12,12,12,48,72, 12,12,12,12,12,12,12,12,12,12,12,12,
		  12, 0,12,12,12,12,12, 0,12, 0,12,12, 12,24,12,12,12,12,12,24,12,24,12,12,
		  12,12,12,12,12,12,12,24,12,12,12,12, 12,24,12,12,12,12,12,12,12,24,12,12,
		  12,12,12,12,12,12,12,36,12,36,12,12, 12,36,12,12,12,12,12,36,12,36,12,12,
		  12,36,12,12,12,12,12,12,12,12,12,12,
		};

		namespace OpCode {
			enum T {
				Continuation = 0,
				Text = 1,
				Binary = 2,
				ConnectionClose = 8,
				Ping = 9,
				Pong = 10
			};
		}
	
		/// <summary>
		/// Describes a lightweight class for describing RFC WebSocket Accept and Response string.
		/// </summary>
		/// <remarks>
		/// For use by a TCP server to perform a handshake over a socket.
		/// This currently only implements support for the RFC WebSocket Text frame.
		/// </remarks>
			
		// String utilities.
		
		/// <summary>
		/// Tests whether this sequence ends with the given sequence.
		/// </summary>
		bool ParseRules::EndsWith(const std::string str, const char c) {
			const int len = str.size();
			if (len == 0) return false;
			return str[len] == c;
		}
			
		/// <summary>
		/// Tests whether this sequence starts with the given character.
		/// </summary>
		bool ParseRules::StartsWith(const std::string str, const char c) {
			const int len = str.size();
			if (len == 0) return false;
			return str[0] == c;
		}
			
		/// <summary>
		/// Trims the given character from the front of the input string.
		/// </summary>
		std::string ParseRules::TrimLeading(const std::string str, const char c) {
			std::ostringstream ss;
			const int len = str.size();
			int i = 0;
			
			while (i < len && str[i] != c) i++;
			for (; i < len; i++) 
				ss << str[i];
			
			return ss.str();
		}
			
		/// <summary>
		/// Generates the required HTTP request headers for making a WebSocket handshake request.
		/// </summary>
		/// <remarks>
		/// For internal use only.
			/// </remarks>
		std::vector<std::string> ParseRules::GenRequiredHeaders() {
			std::vector<std::string> result;
			std::stringstream ss;
			
			/*
				Required headers.
				GET /url HTTP/1.1
				Upgrade: websocket
				Connection: Upgrade
			*/
			
			ss << "GET " << TrimLeading(url, '/') << " HTTP/1.1";
			result.push_back(ss.str());
			ss.clear();
			
			result.push_back("Upgrade: websocket");
			result.push_back("Connection: Upgrade");
			
			return result;
		}
			
		static const std::string base64_chars;
			
		std::string ParseRules::Base64Encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
			std::string ret;
			int i = 0;
			int j = 0;
			unsigned char char_array_3[3];
			unsigned char char_array_4[4];
			
			while (in_len--) {
				char_array_3[i++] = *(bytes_to_encode++);
				if (i == 3) {
					char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] = char_array_3[2] & 0x3f;
					
					for(i = 0; (i <4) ; i++)
						ret += base64_chars[char_array_4[i]];
					i = 0;
				}
			}
			
			if (i)
				{
					for(j = i; j < 3; j++)
					  char_array_3[j] = '\0';
					
					char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] = char_array_3[2] & 0x3f;
					
					for (j = 0; (j < i + 1); j++)
					  ret += base64_chars[char_array_4[j]];
					
					while((i++ < 3))
					  ret += '=';
					
				}
			
			return ret;
		}
		
		
		static uint32_t inline
		decode(uint32_t* state, uint32_t* codep, uint32_t byte) {
			uint32_t type = utf8d[byte];
			
			*codep = (*state != UTF8_ACCEPT) ?
				(byte & 0x3fu) | (*codep << 6) :
				(0xff >> type) & (byte);
			
			*state = utf8d[256 + *state + type];
			return *state;
		}
		
		uint8_t inline
		encode(const char c) {
			return c + 36;
		}
		
		int
		ParseRules::IsUTF8(uint8_t* s) {
			uint32_t codepoint, state = 0;
			
			while (*s)
				decode(&state, &codepoint, *s++);
			
			return state == UTF8_ACCEPT;
		}
		
		ParseRules::ParseRules(const std::string url)
			: Guid6455("258EAFA5-E914-47DA-95CA-C5AB0DC85B11"), url(url), requiredHeaders(GenRequiredHeaders()) {
		}
		
		/// <summary>
		/// Tests whether the given HTTP headers are a valid handshake request.
		/// </summary>
		bool ParseRules::IsWebSocketUpgrade(const std::vector<std::string> headers) const {
			for (auto header : this->requiredHeaders) {
				bool contains = false;
				for (auto line : headers) {
					if (header == line) 
						contains = true;
				}
				
				if (!contains) {
					return false;
				}
			}
				
			return true;
		}
			
		void ParseRules::DisplayMessageDigest(unsigned *message_digest)
		{
			std::ios::fmtflags   flags;
			
			std::cout << '\t';
			
			flags = std::cout.setf(std::ios::hex|std::ios::uppercase,std::ios::basefield);
			std::cout.setf(std::ios::uppercase);
			
			for(int i = 0; i < 5 ; i++)
				{
					std::cout << message_digest[i] << ' ' ;
				}
			
			std::cout << std::endl;
			std::cout.setf(flags);
		}
		
		union uint32_converter {
			uint32_t i;
			uint8_t c[4];
		};
		
		std::string ParseRules::CalcWSAccept6455 (const std::string secWebSocketKey) const {
			std::string key = secWebSocketKey;
			key.append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
			unsigned char hash [20];
			
			websocketpp::sha1::calc (
															 key.c_str(), 
															 key.length(), 
															 hash); 
			
			const std::string enc = websocketpp::base64_encode(hash, 20);
			
			return enc;
		}
			
		bool ParseRules::GetKey(const std::string key, const std::vector<std::string> arr, std::string& result) const {
			for (auto it : arr) {
				auto i = it.find(key);
				if (i != std::string::npos) {
					result = it.substr (i + key.length()); // Substring from key to the end.
					return true;
				}
			}
			// Key was not found.
				
			return false;
		}
			
		/// <summary>
		/// Generates a valid Http WebSocket handshake accept response.
		/// </summary>
		std::string ParseRules::CreateAcceptString6455(const std::string acceptCode) const {
			std::ostringstream os;
			const std::string sep = "\r\n";
			
			os	<< "HTTP/1.1 101 Switching Protocols" << sep 
					<< "Upgrade: websocket" << sep 
					<< "Sec-WebSocket-Accept: " << acceptCode << sep 
					<< "Connection: Upgrade" << sep 
					<< sep ;
			
			return os.str();
		}
			
		/// <summary>
		/// Structures a given payload into a WebSocket final text frame.
		/// See: https://tools.ietf.org/html/rfc6455#page-27
		/// </summary>
		std::string ParseRules::MakeFinalTextFrame(const std::string payload) const {
			std::ostringstream os;
			// (FIN << 7) | (RSV << 4) | opcode.
			// FIN = 1, RSV = 0, opcode = 1 (TextFrame)
			const unsigned char head = 0x81; 
			const size_t len = payload.size();
			int b1 = 0;
			
			os << head; // Write FIN, RSV and Opcode.
			
			if (len <= 125)
				b1 = len;
			else if (len < (1 << 16))
				b1 = 126;
			else 
				b1 = 127;
			
			os << static_cast<unsigned char>(b1); // Put byte
			
			b1 = b1 & 127;
			if (b1 == 126)
				os << htobe16(static_cast<uint16_t>(len)); // Put short, in network order
			else if (b1 == 127)
				os << htobe64(static_cast<uint64_t>(len)); // Put long, in network order
			
			// No masking key.
			
			os << payload;
			return os.str();
		}
			
		/// See: https://tools.ietf.org/html/rfc6455#page-27
		int ParseRules::GetTextFramePayload(const std::vector<char> &frame, std::string& payload) const {
			if (frame.size() < 2) {
				std::cerr << "Short frame" << std::endl;
				return -1;
			}
			
			size_t position = 0;
			
			unsigned char b0 = frame[position++];
			
			const int FIN = ((b0 >> 7) & 1) != 0;
			//const int RSV = (b0 >> 4) & 7;
			if ( !FIN ) {
				std::cerr << "Not FIN" << std::endl;
				return -2;
			}
			
			OpCode::T opcode = (OpCode::T)(b0 & 0x0f);
			if (opcode != OpCode::Text) {
				std::cerr << "Not Text" << std::endl;
				return -3;
			}
			
			unsigned char b1 = frame[position++];
			
			const int MASK = (b1 >> 7) & 1;
			size_t payloadLength;
			int b = b1 & 0x7f;
			
			if (b == 126) {
				uint16_t len = *(reinterpret_cast<const uint16_t *>(&frame[position]));
				position += sizeof(len);
				payloadLength = be16toh(len);
			}
			else if (b == 127) {
				uint64_t len = *(reinterpret_cast<const uint64_t *>(&frame[position]));
				position += sizeof(len);
				payloadLength = be64toh(len);
			}
			else 
				payloadLength = b;

			uint32_t maskingKey = 0x00000000;
			if (MASK) {
				uint32_t mk = *(reinterpret_cast<const uint32_t *>(&frame[position]));
				position += sizeof(mk);
				maskingKey = mk;  // be32toh(mk); leave in same order for masking
			}
			std::cerr << "MaskingKey: " << maskingKey << std::endl;
			size_t i, j;
			for(i = 0; i < payloadLength; i++) {
				unsigned char x = *(reinterpret_cast<const unsigned char *>(&frame[position++]));
				j = i % 4;
				x = x ^ (0xFF & (maskingKey >> (8*(j))));
				payload += x;
			}
			return 0;
		}
		
		const std::string ParseRules::base64_chars = 
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";
		
	}
}










