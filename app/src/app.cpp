#include <iostream>
#include "file.hpp"
#include "name.hpp"
#include "http.hpp"
#include "app.hpp"

HTTP::Response App::returnResponse(HTTP::Request request) {
	HTTP::Response response(200, "OK", "");
	std::string path = request.path;
	if (path == "/" || path == "/upload/png") {
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

void App::handlePost(HTTP::Request request) {
	if (request.headers["Content-Type"] == "image/png") {
		if (request.path != "/upload/png") { return; }
		std::string lastImageID = File::getFile("/.id-png");
		std::string imageID = Name::incrementBase62(lastImageID);
		if (File::clearFile("/.id-png") == "failed") { std::cout << "error clearing file" << std::endl; return; }
		if (File::saveFile("/.id-png", imageID) == "failed") { std::cout << "error writing to /.id" << std::endl; return; }
		std::string path = "/assets/";
		if (File::saveFile(path.append(imageID).append(".png"), request.body) == "success") {
			std::cout << "saved file to " << "/assets/" << imageID << ".png" << std::endl;
		} else {
			std::cout << "it failed lol" << std::endl;
		}	
	}
}
