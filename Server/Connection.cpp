#include "stdafx.h"
#include "Helper.h"
#include "Connection.h"
#include "Service.h"

int initServer(int port, SOCKET &listenSocket, SOCKET *socks, WSAEVENT *events, DWORD &nEvents) {
	WSADATA wsaData;
	SOCKADDR_IN serverAddr;
	int ret;
	if ((ret = WSAStartup((2, 2), &wsaData)) != 0) {
		printf("WSAStartup() failed with error %d\n", ret);
		WSACleanup();
		return 0;
	}
	if ((listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		printf("Failed to get a socket %d\n", WSAGetLastError());
		return 0;
	}
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(port);

	socks[0] = listenSocket;
	events[0] = WSACreateEvent(); //create new events
	nEvents++;

	// Associate event types FD_ACCEPT and FD_CLOSE
	// with the listening socket and newEvent   
	WSAEventSelect(socks[0], events[0], FD_ACCEPT | FD_CLOSE);

	if (bind(listenSocket, (PSOCKADDR)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
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

SOCKET acceptClientSocket(SOCKET &socket, WSANETWORKEVENTS sockEvent) {
	SOCKET clientSocket;
	sockaddr_in clientAddr;
	int clientAddrLen = sizeof(clientAddr);
	if (sockEvent.iErrorCode[FD_ACCEPT_BIT] != 0) {
		printf("FD_ACCEPT failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
		return NULL;
	}

	if ((clientSocket = accept(socket, (sockaddr *)&clientAddr, &clientAddrLen)) == SOCKET_ERROR) {
		printf("accept() failed with error %d\n", WSAGetLastError());
		return NULL;
	}
	return clientSocket;
}

int communicateClientSocket(SOCKET &socket, WSANETWORKEVENTS sockEvent) {
	//Receive message from client
	if (sockEvent.iErrorCode[FD_READ_BIT] != 0) {
		printf("FD_READ failed with error %d\n", sockEvent.iErrorCode[FD_READ_BIT]);
		return -1;
	}
	
	char recvBuff[BUFF_SIZE];
	int ret = receiveMessage(socket, recvBuff);

	//Release socket and event if an error occurs
	if (ret < 0) {
		closesocket(socket);
		return -1;
	}
	else {
		//get message -> resolve message -> create message response -> send response//echo to client
		/*memcpy(sendBuff, recvBuff, ret);
		sendMessage(socket, sendBuff, ret);
		return ret;*/
		Message message = buffToMessage(recvBuff);
		switch (message.opcode) {
		case REGISTER:
			ret = registerService(socket, message);
			break;
		case LOGIN:
			ret = loginService(socket, message);
			break;
		case LOGOUT:
			ret = logoutService(socket, message);
			break;
		case UPLOAD_FILE:
			ret = uploadFileService(socket, message);
			break;
		case DOWNLOAD_FILE:
			ret = downloadFileService(socket, message);
			break;
		default:
			cout << "Request is not found!!!" << endl;
			char nf_mess[BUFF_SIZE] = "Yeu cau khong hop le!";
			char send_buff[BUFF_SIZE];
			messageToBuff(Message(NOT_FOUND, strlen(nf_mess), nf_mess), send_buff);
			sendMessage(socket, send_buff, ret);
			break;
		}
		return ret;
	}
}

int closeClientSocket(SOCKET &socket, WSANETWORKEVENTS sockEvent) {
	if (sockEvent.iErrorCode[FD_CLOSE_BIT] != 0) {
		printf("FD_READ failed with error %d\n", sockEvent.iErrorCode[FD_CLOSE_BIT]);
		return 0;
	}
	//Release socket and event
	closesocket(socket);
	printf("Close successfully socket:  %d\n", socket);
	return 1;
}


