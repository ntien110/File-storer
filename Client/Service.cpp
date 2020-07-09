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
	strcpy(&info_login[strlen(username)], "\n");
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
	strcpy(&info_register[strlen(username)], "\n");
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
	cout << "Dang gui file len ..." << endl;
	do {
		ret = readFile(fileName, index, BUFF_FILE, data);
		data[ret] = 0;
		if (ret == -1) return Message();
		if (ret == 0) break;
		index += ret;
		int r = messageToBuff(Message(TRANFERING, ret, data), sendBuff);
		r = sendMessage( sendBuff, r);
		if (r == -1) return Message();
	} while (ret != 0);
	cout << "Da gui file len thanh cong!!!" << endl;
	ret = messageToBuff(Message(TRANFER_DONE, 4, "Done"), sendBuff);
	ret = sendMessage(sendBuff, ret);
	result = receiveMessage(recvBuff);
	if (result < 0) {
		return Message();
	}
	message_resp = buffToMessage(recvBuff);
	return message_resp;
}

Message downloadFileService(char* tracePath, char* saveLocation) {
	//filePath => tao file rong de luu du 
	int ret, result, index = 0;
	char recvBuff[BUFF_SIZE], sendBuff[BUFF_SIZE];
	Message message_resp;
	//create and send request upload with payload is destination file name
	ret = messageToBuff(Message(DOWNLOAD_FILE, strlen(tracePath), tracePath), sendBuff);
	result = sendMessage(sendBuff, ret);
	if (result == 0) {
		return Message();
	}
	cout << "Dang tai ..." << endl;
	do {
		result = receiveMessage(recvBuff);
		while (result < 0) {
			result = receiveMessage(recvBuff);
		}
		message_resp = buffToMessage(recvBuff);
		if (message_resp.opcode == TRANFER_DONE) break;
		//append message.payload to file
		appendToFile(saveLocation, message_resp.payload, message_resp.length);
	} while (true);
	cout << "Da tai xong!!!" << endl;
	return message_resp;
}

Message createFolderService(char* tracePath, char* nameFolder) {
	int ret, result, index = 0;
	char recvBuff[BUFF_SIZE], sendBuff[BUFF_SIZE], data[BUFF_SIZE];
	Message message_resp;
	//create and send request upload with payload is destination file name
	str_cpy(data, tracePath, strlen(tracePath));
	str_cpy(data + strlen(tracePath), "\n", 1);
	str_cpy(data + strlen(tracePath) + 1, nameFolder, strlen(nameFolder));
	data[strlen(tracePath) + strlen(nameFolder) + 1] = '\0';

	ret = messageToBuff(Message(CREATE_FOLDER, strlen(data), data), sendBuff);
	result = sendMessage(sendBuff, ret);
	if (result == 0) {
		return Message();
	}

	result = receiveMessage(recvBuff);
	if (result < 0) {
		return Message();
	}
	message_resp = buffToMessage(recvBuff);
	return message_resp;
}

Message downloadFolderService(char* tracePath, char* saveLocation) {
	return Message();
};

Message deleteService(char* tracePath){
	int ret, result, index = 0;
	char recvBuff[BUFF_SIZE], sendBuff[BUFF_SIZE];
	Message message_resp;

	ret = messageToBuff(Message(DELETE_DATA, strlen(tracePath), tracePath), sendBuff);
	result = sendMessage(sendBuff, ret);
	if (result == 0) {
		return Message();
	}

	result = receiveMessage(recvBuff);
	if (result < 0) {
		return Message();
	}
	message_resp = buffToMessage(recvBuff);
	return message_resp;
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
		if (result < 0) continue;
		message_resp = buffToMessage(message);
		if (message_resp.opcode == TRANFER_DONE) break;
		str_cpy(metaData + index, message_resp.payload, message_resp.length);
		index = index + message_resp.length;
	} while (true);
	metaData[index] = '\0';
	return index;
}