#include <iostream>
#include "http.hpp"
#include "app.hpp"

HTTP::Response App::returnResponse(HTTP::Request request) {
	HTTP::Response response(200, "OK", "test");
	response.headers["Content-Type"] = "text/plain";

	return response;
}
