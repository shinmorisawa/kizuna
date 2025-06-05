#include <iostream>
#include <string>
#include <vector>
#include "file.hpp"
#include "http.hpp"
#include "app.hpp"

HTTP::Response App::returnResponse(HTTP::Request request, int isTLS) {
	HTTP::Response response(200, "OK", "");
	std::string path = request.path;

	if (response.status_code == 404) {
		std::cout << "got request for: " << path << " from " << request.ip << " but it failed, so returning 404 page :(" << std::endl;
	} else {
		std::cout << "got request for: " << path << " from " << request.ip << " size " << File::sizeOfFile(path) << std::endl;
	}

	if (request.method == "BREW") {
		response.body = "i'm a teapot";
		response.headers["Content-Length"] = "12";
		response.headers["Content-Type"] = "tea/pot";
		response.headers["Caffeine-Content"] = "2mg";
		response.headers["Milk"] = "denied";
		response.headers["Honey"] = "what the fuck is wrong with you";
		response.status_code = 418;
		response.status_text = "I'm a teapot";
		std::cout << "someone tried to brew coffee :(" << std::endl;

		return response;
	}

	if (isTLS != 1) {
		HTTP::Response moved(301, "Moved Permanently", "");
		std::string host = request.headers["Host"];
		moved.headers["Location"] = "https://" + host + request.path;
		return moved;
	}

	if (path == "/") {
		path = "/index.html";
	}

	if (File::isDirectory(path) && path != "/") {
		std::vector<std::string> things = File::getThingsInFolder(path);
		response.body = "<!DOCTYPE html>";
		response.body.append("<html>");
		response.body.append("<head>");
		response.body.append("<title>index of ");
		response.body.append(path);
		response.body.append("</title><meta charset=\"UTF-8\"/><link rel=\"stylesheet\" href=\"/styles.css\"></head>");
		response.body.append("<body><div class=\"parent\"><div class=\"child\">");
		response.body.append("<a class=\"light-text\" href=\"");
		response.body.append(path);
		response.body.append("/..\">..</a><br>");
		for (const auto& s : things) {
			response.body.append("<a class=\"light-text\" href=\"");
			response.body.append(path);
			response.body.append("/");
			response.body.append(s);
			response.body.append("\">");
			response.body.append(s);
			response.body.append("</a>");
			response.body.append("<br>");
		}
		response.body.append("<br/><p class=\"light-text\">kizuna/0.2.0-ruby-chan</p>");
		response.body.append("</div></div></body></html");
		response.headers["Content-Length"] = std::to_string(response.body.size());
		response.headers["Content-Type"] = "text/html";
		return response;
	}

	response.body = File::getFile(path);
	response.headers["Content-Length"] = std::to_string(response.body.size());
	response.headers["Content-Type"] = File::getMIMEType(path);

	if (request.headers["Malformed"] == "yes, please") {
		response.status_code = 400;
		response.status_text = "Bad Request";
		response.body = File::getFile("/400.html");
		response.headers["Content-Length"] = std::to_string(response.body.size());
		response.headers["Content-Type"] = File::getMIMEType("/400.html");
	}

	if (response.body == "failed") {
		response.status_code = 404;
		response.status_text = "Not Found";
		response.body = File::getFile("/404.html");
		response.headers["Content-Length"] = std::to_string(response.body.size());
		response.headers["Content-Type"] = File::getMIMEType("/404.html");
	}


	return response;
}

int App::sizeOfResponse(HTTP::Request request, int isTLS) {
	HTTP::Response response(200, "OK", "");
	std::string path = request.path;

	if (response.status_code == 404) {
		std::cout << "got request for: " << path << " from " << request.ip << " but it failed, so returning 404 page :(" << std::endl;
	} else {
		std::cout << "got request for: " << path << " from " << request.ip << " size " << File::sizeOfFile(path) << std::endl;
	}

	if (request.method == "BREW") {
		response.body = "i'm a teapot";
		response.headers["Content-Length"] = "12";
		response.headers["Content-Type"] = "tea/pot";
		response.headers["Caffeine-Content"] = "2mg";
		response.headers["Milk"] = "denied";
		response.headers["Honey"] = "what the fuck is wrong with you";
		response.status_code = 418;
		response.status_text = "I'm a teapot";
		std::cout << "someone tried to brew coffee :(" << std::endl;
		return std::stoi(response.headers["Content-Length"]);
	}

	if (isTLS != 1) {
		HTTP::Response moved(301, "Moved Permanently", "");
		std::string host = request.headers["Host"];
		moved.headers["Location"] = "https://" + host + request.path;
	}

	if (path == "/") {
		path = "/index.html";
	}

	if (File::isDirectory(path) && path != "/") {
		std::vector<std::string> things = File::getThingsInFolder(path);
		response.body = "<!DOCTYPE html>";
		response.body.append("<html>");
		response.body.append("<head>");
		response.body.append("<title>index of ");
		response.body.append(path);
		response.body.append("</title><meta charset=\"UTF-8\"/><link rel=\"stylesheet\" href=\"/styles.css\"></head>");
		response.body.append("<body><div class=\"parent\"><div class=\"child\">");
		response.body.append("<a class=\"light-text\" href=\"");
		response.body.append(path);
		response.body.append("/..\">..</a><br>");
		for (const auto& s : things) {
			response.body.append("<a class=\"light-text\" href=\"");
			response.body.append(path);
			response.body.append("/");
			response.body.append(s);
			response.body.append("\">");
			response.body.append(s);
			response.body.append("</a>");
			response.body.append("<br>");
		}
		response.body.append("<br/><p class=\"light-text\">kizuna/0.2.0-ruby-chan</p>");
		response.body.append("</div></div></body></html");
		response.headers["Content-Length"] = std::to_string(response.body.size());
		response.headers["Content-Type"] = "text/html";
		return std::stoi(response.headers["Content-Length"]);
	}

	response.body = File::getFile(path);
	response.headers["Content-Length"] = std::to_string(response.body.size());
	response.headers["Content-Type"] = File::getMIMEType(path);

	if (request.headers["Malformed"] == "yes, please") {
		response.status_code = 400;
		response.status_text = "Bad Request";
		response.body = File::getFile("/400.html");
		response.headers["Content-Length"] = std::to_string(response.body.size());
		response.headers["Content-Type"] = File::getMIMEType("/400.html");
	}

	if (response.body == "failed") {
		response.status_code = 404;
		response.status_text = "Not Found";
		response.body = File::getFile("/404.html");
		response.headers["Content-Length"] = std::to_string(response.body.size());
		response.headers["Content-Type"] = File::getMIMEType("/404.html");
	}

	return std::stoi(response.headers["Content-Length"]);
}

std::string App::returnChunkResponse(HTTP::Request request, int isTLS, int chunk) {
	HTTP::Response response(200, "OK", "");
	std::string path = request.path;

	if (chunk > 1) {
		std::string thingy = File::getChunkFromFile(path, chunk);
		return thingy;
	}

	if (response.status_code == 404) {
		std::cout << "got request for: " << path << " from " << request.ip << " but it failed, so returning 404 page :(" << std::endl;
	} else {
		std::cout << "got request for: " << path << " from " << request.ip << " size " << File::sizeOfFile(path) << std::endl;
	}

	if (request.method == "BREW") {
		response.body = "i'm a teapot";
		response.headers["Content-Length"] = "12";
		response.headers["Content-Type"] = "tea/pot";
		response.headers["Caffeine-Content"] = "2mg";
		response.headers["Milk"] = "denied";
		response.headers["Honey"] = "what the fuck is wrong with you";
		response.status_code = 418;
		response.status_text = "I'm a teapot";
		std::cout << "someone tried to brew coffee :(" << std::endl;

		return response.toString();
	}

	if (isTLS != 1) {
		HTTP::Response moved(301, "Moved Permanently", "");
		std::string host = request.headers["Host"];
		moved.headers["Location"] = "https://" + host + request.path;
		return moved.toString();
	}

	if (path == "/") {
		path = "/index.html";
	}

	if (File::isDirectory(path) && path != "/") {
		std::vector<std::string> things = File::getThingsInFolder(path);
		response.body = "<!DOCTYPE html>";
		response.body.append("<html>");
		response.body.append("<head>");
		response.body.append("<title>index of ");
		response.body.append(path);
		response.body.append("</title><meta charset=\"UTF-8\"/><link rel=\"stylesheet\" href=\"/styles.css\"></head>");
		response.body.append("<body><div class=\"parent\"><div class=\"child\">");
		response.body.append("<a class=\"light-text\" href=\"");
		response.body.append(path);
		response.body.append("/..\">..</a><br>");
		for (const auto& s : things) {
			response.body.append("<a class=\"light-text\" href=\"");
			response.body.append(path);
			response.body.append("/");
			response.body.append(s);
			response.body.append("\">");
			response.body.append(s);
			response.body.append("</a>");
			response.body.append("<br>");
		}
		response.body.append("<br/><p class=\"light-text\">kizuna/0.2.0-ruby-chan</p>");
		response.body.append("</div></div></body></html");
		response.headers["Content-Length"] = std::to_string(response.body.size());
		response.headers["Content-Type"] = "text/html";
		return response.toString();
	}

	response.body = File::getFile(path);
	response.headers["Content-Length"] = std::to_string(response.body.size());
	response.headers["Content-Type"] = File::getMIMEType(path);

	if (request.headers["Malformed"] == "yes, please") {
		response.status_code = 400;
		response.status_text = "Bad Request";
		response.body = File::getFile("/400.html");
		response.headers["Content-Length"] = std::to_string(response.body.size());
		response.headers["Content-Type"] = File::getMIMEType("/400.html");
	}

	if (response.body == "failed") {
		response.status_code = 404;
		response.status_text = "Not Found";
		response.body = File::getFile("/404.html");
		response.headers["Content-Length"] = std::to_string(response.body.size());
		response.headers["Content-Type"] = File::getMIMEType("/404.html");
	}


	return response.toString();
}

void App::handlePost([[maybe_unused]]HTTP::Request request) {
}
