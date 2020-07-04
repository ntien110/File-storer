#include "stdafx.h"
#include "Connection.h"

int connectToServer(int port, char *ip_addr, SOCKET &client) {
	//Step 1: Create WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Version is not support.\n");
	}
	//Step 2: Construct socket
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//(optional) Set time-out for receiving
	int tv = 500000; //Time-out interval: 5000ms
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO,
		(const char*)(&tv), sizeof(int));
	//Step 3: Bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip_addr);
	//Step 4: Request to connect server
	if (connect(client, (sockaddr *)&serverAddr,
		sizeof(serverAddr))) {
		printf("Error! Cannot connect server.\n");
		return 0;
	}
	return 1;
}