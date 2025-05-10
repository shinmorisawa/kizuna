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


