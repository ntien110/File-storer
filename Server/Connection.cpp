#include "stdafx.h"
#include "Connection.h"

int initServer(int port, SOCKET &listenSocket) {
	WSADATA wsaData;
	SOCKADDR_IN InternetAddr;
	int ret;
	if ((ret = WSAStartup((2, 2), &wsaData)) != 0) {
		printf("WSAStartup() failed with error %d\n", ret);
		WSACleanup();
		return 0;
	}
	if ((listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
		printf("Failed to get a socket %d\n", WSAGetLastError());
		return 0;
	}
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(port);
	if (bind(listenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR) {
		printf("bind() failed with error %d\n", WSAGetLastError());
		return 0;
	}
	if (listen(listenSocket, 5)) {
		printf("listen() failed with error %d\n", WSAGetLastError());
		return 0;
	}
	printf("Server started!\n");
	return 1;
}

int initAcceptEvent(WSAEVENT &AcceptEvent) {
	if ((AcceptEvent = WSACreateEvent()) == WSA_INVALID_EVENT) {
		printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
		return 0;
	}
	printf("WSACreateEvent() is OK!\n");
	return 1;
}

int acceptFromClient(SOCKET &listenSocket, SOCKET &clientSocket, WSAEVENT &acceptEvent) {
	clientSocket = accept(listenSocket, NULL, NULL);
	if (WSASetEvent(acceptEvent) == FALSE) {
		printf("WSASetEvent() failed with error %d\n", WSAGetLastError());
		return 0;
	}
	printf("WSASetEvent() should be working!\n");
	return 1;
}