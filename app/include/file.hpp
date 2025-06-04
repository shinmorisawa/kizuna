#pragma once
#include <string>
#include <vector>

namespace File {
	std::string getFile(std::string path);
	int sizeOfFile(std::string path);
	std::vector<std::string> getThingsInFolder(std::string path);
	std::string getMIMEType(std::string path);
	std::string getChunkFromFile(std::string path, int chunk);
	bool isDirectory(std::string path);
}
