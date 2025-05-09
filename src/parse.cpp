#include <iostream>
#include <string>
#include <map>
#include "parse.hpp"

HTTP::Response HTTP::parseRequest(std::string request) {
	/* sorry for being esoteric 
	 * this top one is for the request line */
	std::string request_line = request;
	std::string::size_type end = request_line.find("\r\n");
	request_line = request_line.substr(0, end);
	std::cout << "request line: " << std::endl << request_line << std::endl;

	/* request headers */
	std::string request_headers = request;
	std::string::size_type beg = request_headers.find("\r\n");
	end = request_headers.find("\r\n\r\n");
	request_headers = request_headers.substr(beg+2, end-4);
	std::cout << "headers: " << std::endl << request_headers;

	/* request body */
	std::string request_body = request;
	beg = request_body.find("\r\n\r\n");
	end = request_body.find('\0');
	request_body = request_body.substr(beg+4, end);
	std::cout << "body: " << std::endl << request_body << std::endl << std::endl;
	/* TODO: implement parser */
	HTTP::Response response;
	
	response.version = "HTTP/1.1";
	response.status_code = "200";
	response.status_text = "OK";
	response.headers = {{}};
	response.body = "";

	return response;
}
