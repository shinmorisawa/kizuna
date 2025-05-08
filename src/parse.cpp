#include <iostream>
#include <string>
#include "parse.hpp"

HTTP::HttpRequest HTTP::parseRequest(std::string request) {
	std::string request_line = request;

	std::string::size_type end = request_line.find("\r\n");
	request_line = request_line.substr(0, end);
	std::cout << request_line;

	/* TODO: implement parser */

	return HTTP::blankRequest();
}

/* for getting the private vars from the class */
std::string HTTP::HttpRequest::getMethod()  { return method;  }
std::string HTTP::HttpRequest::getPath()    { return path;    }
std::string HTTP::HttpRequest::getVersion() { return version; }
std::string HTTP::HttpRequest::getBody()    { return body;    }

/* for setting the private vars from the class */
void HTTP::HttpRequest::setMethod(std::string m)  { method = m;  }
void HTTP::HttpRequest::setPath(std::string p)    { path = p;    }
void HTTP::HttpRequest::setVersion(std::string v) { version = v; } 
void HTTP::HttpRequest::setBody(std::string b)    { body = b;    }

/* just a blank request for testing purposes */
HTTP::HttpRequest HTTP::blankRequest() {
	HTTP::HttpRequest blank;

	blank.setMethod("0");
	blank.setPath("0");
	blank.setVersion("0");
	blank.setBody("0");

	return blank;
}
