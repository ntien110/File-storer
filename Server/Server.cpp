#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>
#include "Connection.h"
#include "FileManagement.h"
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)

#define PORT 5500

using namespace std;

vector<User> userList;
map<int, int> userLogged;
map<int, int> userLogInAttemp;

int main(int argc, char **argv) {
	SOCKET listenSocket;
	SOCKET		socks[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT	events[WSA_MAXIMUM_WAIT_EVENTS];
	bool		userLogged[WSA_MAXIMUM_WAIT_EVENTS];
	DWORD		nEvents = 0;
	DWORD		index;
	int ret;

	//init file management
	setAppPath();
	createDirectoryByPath(appPath);
	
	char* accountFileName = new char[256];
	strcpy(accountFileName, appPath);
	addToPath(accountFileName, "account.txt");

	if (getFileLength(accountFileName) < 0) {
		writeToFile(accountFileName, "", 0);
	}

	ret = initServer(PORT, listenSocket, socks, events, nEvents);
	if (ret == 0) return 0;

	SOCKET clientSocket;
	WSANETWORKEVENTS sockEvent;

	for (int i = 1; i < WSA_MAXIMUM_WAIT_EVENTS; i++) {
		socks[i] = 0;
	}

	while (1) {
		//init and wait for network events on all socket
		index = WSAWaitForMultipleEvents(nEvents, events, FALSE, WSA_INFINITE, FALSE);
		if (index == WSA_WAIT_FAILED) {
			printf("WSAWaitForMultipleEvents() failed with error %d\n", WSAGetLastError());
			break;
		}
		index = index - WSA_WAIT_EVENT_0;
		WSAEnumNetworkEvents(socks[index], events[index], &sockEvent );

		//have new connection from one client
		if (sockEvent.lNetworkEvents & FD_ACCEPT) {
			clientSocket = acceptClientSocket(socks[index], sockEvent);
			//Add new socket into socks array
			if (clientSocket == NULL) break;
			if (nEvents == WSA_MAXIMUM_WAIT_EVENTS) {
				printf("Too many clients.\n");
				closesocket(clientSocket);
			}
			else {
				for (int i = 1; i < WSA_MAXIMUM_WAIT_EVENTS; i++)
					if (socks[i] == 0) {
						socks[i] = clientSocket;
						events[i] = WSACreateEvent();
						WSAEventSelect(socks[i], events[i], FD_READ | FD_CLOSE);
						nEvents++;
						break;
					}
				//reset event
				printf("Connect successfully socket: %d.\n", clientSocket);
			}
			WSAResetEvent(events[index]);
		}
		//request from client
		if (sockEvent.lNetworkEvents & FD_READ) {
			int ret = communicateClientSocket(socks[index], sockEvent);
			if (ret <= 0) {
				socks[index] = 0;
				WSACloseEvent(events[index]);
				nEvents--;
			} else WSAResetEvent(events[index]);
		}
		//close client socket
		if (sockEvent.lNetworkEvents & FD_CLOSE) {
			int result = closeClientSocket(socks[index], sockEvent);
			socks[index] = 0;
			WSACloseEvent(events[index]);
			nEvents--;
		}
	}
	return 0;
}