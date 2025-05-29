#pragma once

#include <string>

namespace File {
	std::string getFile(std::string path);
	std::string getMIMEType(std::string path);
	std::string saveFile(std::string path, std::string data);
	std::string clearFile(std::string path);
}
