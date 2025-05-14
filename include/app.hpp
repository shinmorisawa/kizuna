#pragma once
#include "http.hpp"

namespace App {
	HTTP::Response returnResponse(HTTP::Request request);
	void handlePost(HTTP::Request request);
}
