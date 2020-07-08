#include "stdafx.h"
#include "Helper.h"
#include "Service.h"
#include "string.h"
#include "FileManagement.h"
#include <conio.h>

#pragma warning(disable:4996)

/*
	Logical login code
	input: (char *username, char *password)
	output: Message - message response from server
*/
Message loginService(char *username, char *password) {
	int result, length;
	char info_login[BUFF_SIZE];
	char message[BUFF_SIZE];
	Message message_resp;

	strcpy(info_login, username);
	strcpy(&info_login[strlen(username)], " ");
	strcpy(&info_login[strlen(username) + 1], password);
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
	message_resp = buffToMessage(message);
	return message_resp;
};

/*
	Logical logout code
	input: 
	output: Message message response from server
*/
Message logoutService() {
	int ret, result;
	char message[BUFF_SIZE];
	Message message_resp;
	ret = messageToBuff(Message(LOGOUT, 6, "logout"), message);
	result = sendMessage(message, ret);
	if (result == 0) {
		return Message();
	}
	result = receiveMessage(message);
	if (result < 0) {
		return Message();
	}
	message_resp = buffToMessage(message);
	return message_resp;
};

/*
	Logical register code
	input: (char *username, char *password)
	output: Message message response from server
*/
Message registerService(char *username, char *password) {
	int result, length;
	char info_register[BUFF_SIZE];
	char message[BUFF_SIZE];
	Message message_resp;

	strcpy(info_register, username);
	strcpy(&info_register[strlen(username)], " ");
	strcpy(&info_register[strlen(username) + 1], password);
	length = strlen(username) + 1 + strlen(password);
	messageToBuff(Message(REGISTER, length, info_register), message);
	result = sendMessage(message, 1 + 2 + length);
	if (result == 0) {
		return Message();
	}
	result = receiveMessage(message);
	if (result < 0) {
		return Message();
	}
	message_resp = buffToMessage(message);
	return message_resp;
};

Message uploadFileService(char *tracePath, char *fileName) {
	int ret, result, index = 0;
	char recvBuff[BUFF_SIZE], sendBuff[BUFF_SIZE], data[BUFF_SIZE];
	Message message_resp;
	//create and send request upload with payload is destination file name
	str_cpy(data, tracePath, strlen(tracePath));
	str_cpy(data + strlen(tracePath), "\n", 1);
	str_cpy(data+strlen(tracePath)+1, fileName, strlen(fileName));
	data[strlen(tracePath) + strlen(fileName)+1] = '\0';

	if (getFileLength(fileName) < 0) {
		return Message(204, 18 ,"File khong ton tai");
	}

	ret = messageToBuff(Message(UPLOAD_FILE, strlen(data), data), sendBuff);
	result = sendMessage(sendBuff, ret);
	if (result == 0) {
		return Message();
	}

	do {
		ret = readFile(fileName, index, 1024, data);
		data[ret] = 0;
		if (ret == -1) return Message();
		if (ret == 0) break;
		index += ret;
		int r = messageToBuff(Message(TRANFERING, ret, data), sendBuff);
		r = sendMessage( sendBuff, r);
		if (r == -1) return Message();
	} while (ret != 0);
	ret = messageToBuff(Message(TRANFER_DONE, 4, "Done"), sendBuff);
	ret = sendMessage(sendBuff, ret);
	result = receiveMessage(recvBuff);
	if (result < 0) {
		return Message();
	}
	message_resp = buffToMessage(recvBuff);
	return message_resp;
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

int getMetadataService(char *metaData) {
	int ret, result, index = 0;
	char message[BUFF_SIZE];
	Message message_resp;
	ret = messageToBuff(Message(GET_META_FILE, 0, ""), message);
	result = sendMessage(message, ret);
	if (result == 0) {
		return 0;
	}
	do {
		result = receiveMessage(message);
		if (result < 0) {
			return result;
		}
		message_resp = buffToMessage(message);
		if (message_resp.opcode == TRANFER_DONE) break;
		str_cpy(metaData + index, message_resp.payload, message_resp.length);
		index = index + message_resp.length;
	} while (true);
	metaData[index] = '\0';
	return index;
}