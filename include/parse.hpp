#ifndef PARSE_HPP
#define PARSE_HPP

#include <map>
#include <string>

namespace HTTP {
	class Request {
		public:
			std::string getMethod();
			std::string getPath();
			std::string getVersion();
			std::string getBody();

			void setMethod(std::string m);
			void setPath(std::string p);
			void setVersion(std::string v);
			void setBody(std::string b);

			std::map<std::string, std::string> headers;
		private:
			std::string method;
			std::string path;
			std::string version;
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

	Request parseRequest(std::string request);
	Request blankRequest();
}

#endif
