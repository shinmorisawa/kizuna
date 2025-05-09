#ifndef PARSE_HPP
#define PARSE_HPP

#include <map>
#include <string>

namespace HTTP {
	class Request {
		public:
			std::string method;
			std::string path;
			std::string version;
			std::map<std::string, std::string> headers;	
			std::string body;
	};

	class Response {
		public:
			std::string version;
			std::string status_code;
			std::string status_text;
			std::map<std::string, std::string> headers;
			std::string body;
	};

	Response parseRequest(std::string request);
}

#endif
