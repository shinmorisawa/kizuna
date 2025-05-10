#include <iostream>
#include <string>
#include <map>
#include "http.hpp"

HTTP::Request HTTP::parseRequest(std::string request) {
	/* sorry for being esoteric 
	 * this top one is for the request line */
	std::string request_line = request;
	std::string::size_type end = request_line.find("\r\n");
	request_line = request_line.substr(0, end);

	/* request headers */
	std::string request_headers = request;
	std::string::size_type beg = request_headers.find("\r\n");
	end = request_headers.find("\r\n\r\n");
	request_headers = request_headers.substr(beg+2, end-4);

	/* request body */
	std::string request_body = request;
	beg = request_body.find("\r\n\r\n");
	end = request_body.find('\0');
	request_body = request_body.substr(beg+4, end);

	/* TODO: implement parser (80% done) */
	HTTP::Request parsed_request;

	std::cout << request_line << std::endl;

	end = request_line.find(" ");
	parsed_request.method = request_line.substr(0, end);
	request_line.erase(0, end + 1);
	end = request_line.find(" ");
	parsed_request.path = request_line.substr(0, end);
	request_line.erase(0, end + 1);
	end = request_line.find("\r\n");
	parsed_request.version = request_line.substr(0, end);
	std::cout << parsed_request.method << "a" << parsed_request.path << "b" << parsed_request.version << "c" << std::endl;

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
		std::cout << key << ": " << value << "\n";
	}
	
	parsed_request.headers = headers;
	parsed_request.body = request_body;

	return parsed_request;
}
