// Client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "stdio.h"
#include "conio.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#include <string>
#include "helper.h"

#define BUFF_SIZE 2048
#define PORT 5500
#define IPADDR "127.0.0.1"

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int main(int argc, char *argv[]) {
	//Step 1: Create WinSock
	int port = PORT;
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData)) {
		printf("Version is not support.\n");
	}
	//Step 2: Construct socket
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//(optional) Set time-out for receiving
	int tv = 5000; //Time-out interval: 5000ms
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO,
		(const char*)(&tv), sizeof(int));
	//Step 3: Bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(IPADDR);
	//Step 4: Request to connect server
	if (connect(client, (sockaddr *)&serverAddr,
		sizeof(serverAddr))) {
		printf("Error! Cannot connect server.\n");
		_getch();
		return 0;
	}
	//Step 5: Communicate with server
	char buff[BUFF_SIZE], response[BUFF_SIZE];
	int ret;
	//Send message
	do {
		gets_s(buff, BUFF_SIZE);
		ret = sendMessage(client, buff, strlen(buff));
		if (ret == SOCKET_ERROR)
			printf("Error! Cannot send mesage.\n");
		//Receive echo message
		ret = receiveMessage(client, response);
		if (ret == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT)
				printf("Time-out!\n");
			else printf("Error! Cannot receive message.\n");
		}
		else {
			response[ret] = '\0';
			if (strcmp(response, "\0") == 0) break;
			printf("%s\n", response);
		}
	} while (true);
	//Step 6: Close socket
	closesocket(client);
	//Step 7: Terminate Winsock
	WSACleanup();
	return 0;
}

