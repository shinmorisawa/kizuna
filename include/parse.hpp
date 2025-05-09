#ifndef PARSE_HPP
#define PARSE_HPP

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
		private:
			std::string method;
			std::string path;
			std::string version;
			std::string body;
	};

	Request parseRequest(std::string request);
	Request blankRequest();
}

#endif
