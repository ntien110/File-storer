#include "stdafx.h"
#include "Helper.h"
#include "Service.h"

Message loginService(char *username, char *password) {
	int result, length;
	char info_login[BUFF_SIZE];
	char message[BUFF_SIZE];

	str_cpy(info_login, username);
	str_cpy(&info_login[strlen(username)], " ");
	str_cpy(&info_login[strlen(username) + 1], password);
	length = strlen(username) + 1 + strlen(password);
	messageToBuff(Message(LOGIN, length, info_login), message);
	result = sendMessage(message, 1 + 2 + length);
	if (result == 0) {
		return Message();
	}
	result = receiveMessage(message);
	if (result < 0) {
		return Message();
	}
	Message response_message = buffToMessage(message);
	return response_message;
};

Message logoutService() {
	char *buff = "Register";
	char response[BUFF_SIZE];
	int ret = sendMessage(buff, strlen(buff));
	if (ret == SOCKET_ERROR)
		printf("Error! Cannot send mesage.\n");
	ret = receiveMessage(response);
	if (ret == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAETIMEDOUT)
			printf("Time-out!\n");
		else printf("Error! Cannot receive message.\n");
	}
	else {
		response[ret] = '\0';
		printf("%s\n", response);
	}
};

Message registerService(char *username, char *password) {
	char *buff = "Register";
	char response[BUFF_SIZE];
	int ret = sendMessage(buff, strlen(buff));
	if (ret == SOCKET_ERROR)
		printf("Error! Cannot send mesage.\n");
	ret = receiveMessage(response);
	if (ret == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAETIMEDOUT)
			printf("Time-out!\n");
		else printf("Error! Cannot receive message.\n");
	}
	else {
		response[ret] = '\0';
		printf("%s\n", response);
	}
};

Message uploadFileService(char *file_namet) {
	char *buff = "Upload file";
	char response[BUFF_SIZE];
	int ret = sendMessage(buff, strlen(buff));
	if (ret == SOCKET_ERROR)
		printf("Error! Cannot send mesage.\n");
	/*bool ret1 = TransmitFile(client, fp, 0, 0, NULL, NULL, TF_USE_DEFAULT_WORKER);
	if (!ret1)
	printf("Error! Cannot send mesage %d.\n", WSAGetLastError());*/
	//Receive echo message
	ret = receiveMessage(response);
	if (ret == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAETIMEDOUT)
			printf("Time-out!\n");
		else printf("Error! Cannot receive message.\n");
	}
	else {
		response[ret] = '\0';
		printf("%s\n", response);
	};
}

Message downloadFileService(char *file_name) {
	char *buff = "Download file";
	char response[BUFF_SIZE];
	int ret = sendMessage(buff, strlen(buff));
	if (ret == SOCKET_ERROR)
		printf("Error! Cannot send mesage.\n");
	/*bool ret1 = TransmitFile(client, fp, 0, 0, NULL, NULL, TF_USE_DEFAULT_WORKER);
	if (!ret1)
	printf("Error! Cannot send mesage %d.\n", WSAGetLastError());*/
	//Receive echo message
	ret = receiveMessage(response);
	if (ret == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAETIMEDOUT)
			printf("Time-out!\n");
		else printf("Error! Cannot receive message.\n");
	}
	else {
		response[ret] = '\0';
		printf("%s\n", response);
	};
}