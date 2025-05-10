#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include "file.hpp"

namespace fs = std::filesystem;
fs::path base_path = "/var/www/kizuna";

std::string File::getTextFile(std::string path) {
	if (!path.empty() && path[0] == '/') path = path.substr(1);
	fs::path full_path = base_path / path;

	std::ifstream file(full_path);

	if (!file) {
		std::cout << "couldn't open file " << full_path << std::endl;
		return "failed";
	}

	std::cout << path << std::endl;

	std::stringstream buf;
	buf << file.rdbuf();
	return buf.str();
}

std::string File::getMIMEType(std::string path) {
	if (path.ends_with(".html")) return "text/html";
	if (path.ends_with(".css")) return "text/css";
	if (path.ends_with(".js")) return "application/javascript";
	if (path.ends_with(".png")) return "image/png";
	if (path.ends_with(".jpg") || path.ends_with(".jpeg")) return "image/jpeg";
	if (path.ends_with(".gif")) return "image/gif";
	if (path.ends_with(".svg")) return "image/svg+xml";
	if (path.ends_with(".flac")) return "audio/flac";
	if (path.ends_with(".ogg")) return "audio/ogg";
	return "application/octet-stream";
}


