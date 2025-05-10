#pragma once
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
			std::string version = "HTTP/1.1";
			int status_code = 200;
			std::string status_text = "OK";
			std::map<std::string, std::string> headers;
			std::string body;

			Response() {}

			Response(int status_code, const std::string &text, const std::string &body_content = "")
				: status_code(status_code), status_text(text), body(body_content) {
				version = "HTTP/1.1";
				headers["Content-Length"] = std::to_string(body.size());
				headers["Content-Type"] = "text/plain";
				headers["Server"] = "kizuna/0.0.1-prerelease";
			}

			std::string toString() const {
				std::string res = version + " " + std::to_string(status_code) + " " + status_text + "\r\n";
				for (const auto &[key, val] : headers) {
					res += key + ": " + val + "\r\n";
				}
				res += "\r\n" + body;
				return res;
			}
	};

	Request parseRequest(std::string request);
}
