#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include "http.hpp"

HTTP::Request HTTP::parseRequest(std::string request) {
	HTTP::Request wrong;
	wrong.method = "GET";
	wrong.path = "/";
	wrong.version = "HTTP/1.1";
	wrong.headers["Malformed"] = "yes, please";
	wrong.body = "";
	wrong.ip = nullptr;

	/* sorry for being esoteric 
	 * this top one is for the request line */
	std::string request_line = request;
	std::string::size_type end = request_line.find("\r\n");
	if (request_line.size() > 1) {
		request_line = request_line.substr(0, end);
	} else {
		return wrong;
	}

	/* request headers */
	std::string request_headers = request;
	std::string::size_type beg = request_headers.find("\r\n");
	end = request_headers.find("\r\n\r\n");
	if (request_headers.size() > 1) {
		request_headers = request_headers.substr(beg+2, end-4);
	} else {
		return wrong;
	}

	/* request body */
	std::string request_body = request;
	beg = request_body.find("\r\n\r\n");
	end = request_body.find('\0');
	if (request_body.size() > 1) {
		request_body = request_body.substr(beg+4, end);
	} else {
		return wrong;
	}

	/* parser */
	HTTP::Request parsed_request;

	end = request_line.find(" ");
	parsed_request.method = request_line.substr(0, end);
	request_line.erase(0, end + 1);
	end = request_line.find(" ");
	parsed_request.path = request_line.substr(0, end);
	parsed_request.path = HTTP::URLDecode(parsed_request.path);
	request_line.erase(0, end + 1);
	end = request_line.find("\r\n");
	parsed_request.version = request_line.substr(0, end);

	/* check for malformed requests */
	if (parsed_request.version != "HTTP/1.1") {
		return wrong;
	}
	if (parsed_request.method != "GET" && parsed_request.method != "POST") {
		return wrong;
	}
	/* don't ask me how it works it's just black magic */
	std::map<std::string, std::string> headers;	
	while (!request_headers.empty()) {
		std::string::size_type sep = request_headers.find(": ");
		if (sep == std::string::npos) break;

		std::string key = request_headers.substr(0, sep);
		request_headers.erase(0, sep + 2);

		std::string::size_type line_end = request_headers.find("\r\n");
		if (line_end == std::string::npos) break;

		std::string value = request_headers.substr(0, line_end);
		request_headers.erase(0, line_end + 2);

		headers[key] = value;
	}
	
	parsed_request.headers = headers;
	parsed_request.body = request_body;

	return parsed_request;
}

std::string HTTP::URLDecode(const std::string& str) {
	std::ostringstream parsed;
	for (size_t i = 0; i < str.length(); ++i) {
		if (str[i] == '%' && i + 2 < str.length()) {
			std::string hex = str.substr(i + 1, 2);
			parsed << static_cast<char>(std::stoi(hex, nullptr, 16));
			i += 2;
		} else if (str[i] == '+') {
			parsed << ' ';
		} else {
			parsed << str[i];
		}
	}
	return parsed.str();
}
