#pragma once

#include <string>

namespace File {
	std::string getTextFile(std::string path);
	std::string getMIMEType(std::string path);
}
