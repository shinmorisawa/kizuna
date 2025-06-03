#include <thread>
#include "socket.hpp"
#include "openssl/ssl.h"
#include "openssl/err.h"

int main() {
	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

	std::thread([]() { Socket::initTLSSocket(); }).detach();
	std::thread([]() { Socket::initSocket();    }).detach();

	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	return 0;
}
