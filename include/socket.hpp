#pragma once
#include "openssl/ssl.h"

namespace Socket {
	void startAcceptingClients(int serverSocket);
	void startAcceptingTLSClients(SSL_CTX* ctx, int serverSocket);
	void initSocket();
	void initTLSSocket();
	SSL_CTX* createContext();
	void configureContext(SSL_CTX* ctx);
}
