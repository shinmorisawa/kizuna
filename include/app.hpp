#pragma once
#include <string>
#include <openssl/ssl.h>
#include "http.hpp"

namespace App {
	HTTP::Response returnResponse(HTTP::Request request, int isTLS);
	int sizeOfResponse(HTTP::Request request, int isTLS);
	void sendBigResponse(HTTP::Request request, SSL* ssl);
	void handlePost(HTTP::Request request);
}
