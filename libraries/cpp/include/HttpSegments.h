#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <sstream>

namespace Net {
	namespace Http {
	
		/// <summary>
		/// Describes a lightweight class for parsing the basic HTTP format.
		/// </summary>
		class HttpSegments {
		public:
			static const std::string HeaderBodySeperator;
			static const std::string HeaderSeparator;
		
			std::vector<std::string> HeaderSegments;
			std::string Body;
			
			/// <summary>
			/// Attempts to parse the HTTP header segments from a chunk of data.
			/// </summary>
			static std::string ParseHeaders(const std::string dataIn, std::vector<std::string>& headers) {
				char* data = new char[dataIn.length()];
				strcpy(data, dataIn.c_str());
				char* token;
				
				token = strtok(data, HeaderSeparator.c_str());
				while (token != NULL) {
					headers.push_back(std::string(token));
					token = strtok(NULL, HeaderSeparator.c_str());	
				}
				
				delete [] data;
				
				return "";
			}
			
			/// <summary>
			/// Attempts to parse the HTTP header segments and body from a chunk of dat.
			/// </summary>
			/// <remarks>
			/// Does not check for tailing HTTP line deliminator.
			/// </remarks>
			static std::string Parse(const std::vector<char> &dataIn, HttpSegments& out) {
				std::string headerData;
				char* token;
				char* data = new char[dataIn.size()+1];
				std::string err;
				
				strncpy(data, dataIn.data(), dataIn.size());  // OK to treat as c-string, since is HTTP data here.
				data[dataIn.size()] = 0;
				
				token = strtok(data, HeaderSeparator.c_str());
				while (token != NULL && std::string(token) != "") {
					out.HeaderSegments.push_back(token);
					token = strtok(NULL, HeaderSeparator.c_str());
				}
				
				// Pull the Body data.
				token = strtok(NULL, HeaderBodySeperator.c_str());
				if (token == NULL) out.Body = "";
				else out.Body = token;
				
				// todo: Check for trailing deliminator.
				/*
				token = strtok(NULL, HeaderSeparator);
				if (token != NULL) return "Error: Expected end of data.";
				*/
				
				// Parse the headers directly into the resulting structure.
				/*err = ParseHeaders(headerData, out.HeaderSegments);
				if (err != "") {
					delete [] data;
					return err;
				}*/
				
				delete [] data;
				return "";
			}
		};
		
		const std::string HttpSegments::HeaderBodySeperator = "\r\n\r\n";
		const std::string HttpSegments::HeaderSeparator = "\r\n";
	}
}


















