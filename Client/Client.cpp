// Client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "conio.h"
#include "helper.h"
#include "Connection.h"
#include <string>
#include "mswsock.h"

#pragma comment(lib, "Mswsock.lib")

#define BUFF_SIZE 2048
#define PORT 5500
#define IPADDR "127.0.0.1"

using namespace std;

int main(int argc, char *argv[]) {
	SOCKET client;
	int result = connectToServer(PORT, IPADDR, client);
	if (result == 0) return 0;
	//Step 5: Communicate with server
	char buff[BUFF_SIZE], response[BUFF_SIZE];
	int ret;
	//Send message
	FILE *fp;
	fopen_s(&fp, "log.txt", "r");
	do {
		gets_s(buff, BUFF_SIZE);
		ret = sendMessage(client, buff, strlen(buff));
		if (ret == SOCKET_ERROR)
			printf("Error! Cannot send mesage.\n");
		/*bool ret1 = TransmitFile(client, fp, 0, 0, NULL, NULL, TF_USE_DEFAULT_WORKER);
		if (!ret1)
			printf("Error! Cannot send mesage %d.\n", WSAGetLastError());*/
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

