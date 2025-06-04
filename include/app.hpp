#pragma once
#include <string>
#include "http.hpp"

namespace App {
	HTTP::Response returnResponse(HTTP::Request request, int isTLS);
	int sizeOfResponse(HTTP::Request request, int isTLS);
	std::string returnChunkResponse(HTTP::Request request, int isTLS, int chunk);
	void handlePost(HTTP::Request request);
}
