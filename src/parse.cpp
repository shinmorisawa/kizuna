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

	/* TODO: implement parser (20% done) */
	HTTP::Request parsed_request;

	end = request_line.find(" ");
	parsed_request.method = request_line.substr(0, end);
	request_line.erase(0, end);
	end = request_line.find(" ");
	parsed_request.path = request_line.substr(0, end);
	request_line.erase(0, end);
	end = request_line.find("\r\n");
	parsed_request.version = request_line.substr(0, end);

	return parsed_request;
}
