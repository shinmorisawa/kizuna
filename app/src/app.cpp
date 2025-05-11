#include <iostream>
#include "file.hpp"
#include "http.hpp"
#include "app.hpp"

HTTP::Response App::returnResponse(HTTP::Request request) {
	HTTP::Response response(200, "OK", "");
	std::string path = request.path;
	if (path == "/") {
		path = "/index.html";
	}
	response.body = File::getFile(path);
	response.headers["Content-Length"] = std::to_string(response.body.size());
	response.headers["Content-Type"] = File::getMIMEType(path);

	if (response.body == "failed") {
		response.status_code = 404;
		response.status_text = "Not Found";
		response.body = File::getFile("/404.html");
		response.headers["Content-Length"] = std::to_string(response.body.size());
		response.headers["Content-Type"] = File::getMIMEType("/404.html");
	}

	if (response.status_code == 404) {
		std::cout << "got request for: " << path << " but it failed, so returning 404 page :(" << std::endl;
	} else {
		std::cout << "got request for: " << path << std::endl;
	}

	return response;
}
