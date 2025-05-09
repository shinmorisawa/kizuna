#include <iostream>
#include <string>
#include <map>
#include "parse.hpp"

HTTP::Request HTTP::parseRequest(std::string request) {
	std::string request_line = request;

	std::string::size_type end = request_line.find("\r\n");
	request_line = request_line.substr(0, end);

	

	return HTTP::blankRequest();
}

/* for getting the private vars from the class */
std::string HTTP::Request::getMethod()  { return method;  }
std::string HTTP::Request::getPath()    { return path;    }
std::string HTTP::Request::getVersion() { return version; }
std::string HTTP::Request::getBody()    { return body;    }

/* for setting the private vars from the class */
void HTTP::Request::setMethod(std::string m)  { method = m;  }
void HTTP::Request::setPath(std::string p)    { path = p;    }
void HTTP::Request::setVersion(std::string v) { version = v; } 
void HTTP::Request::setBody(std::string b)    { body = b;    }

/* just a blank request for testing purposes */
HTTP::Request HTTP::blankRequest() {
	HTTP::Request blank;

	blank.setMethod("0");
	blank.setPath("0");
	blank.setVersion("0");
	blank.setBody("0");
	blank.headers = {{}};

	return blank;
}
