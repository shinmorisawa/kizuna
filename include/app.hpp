#pragma once
#include "http.hpp"

namespace App {
	HTTP::Response returnResponse(HTTP::Request request, int isTLS);
	void handlePost(HTTP::Request request);
}
