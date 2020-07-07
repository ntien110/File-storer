#include "stdafx.h"
#include "helper.h"
#include <string.h>
#include "FileManagement.h"
#include <fstream> 

#pragma warning(disable:4996)

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int sendMessage(SOCKET client, char *message, int length) {
	int ret, index = 0;
	char len[BUFF_SIZE];
	_itoa_s(length, len, 10);
	ret = send(client, len, BUFF_SIZE, 0);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	cout << "Do dai: " << length << endl;
	while (length > 0) {
		ret = send(client, &message[index], length, 0);
		if (ret == SOCKET_ERROR) continue;
		index += ret;
		length -= ret;
	}
	cout << "Da gui: " << message << endl;
	return index;
}

//length do dai du lieu muon nhan
int receiveMessage(SOCKET client, char *message) {
	char len[BUFF_SIZE];
	int ret, index = 0;
	ret = recv(client, len, BUFF_SIZE, 0);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	int length = atoi(len);
	while (length > 0) {
		ret = recv(client, &message[index], length, 0);
		if (ret == SOCKET_ERROR) {
			continue;
		}
		index += ret;
		length -= ret;
	}
	message[index] = '\0';
	cout << "Da nhan: " << message << endl;
	return index;
}

void str_cpy(char *dest, char *src) {
	while (*src) {
		*(dest++) = *(src++);
	}
	*dest = '\0';
}

Message buffToMessage(char *buff) {
	Message message;
	//memcpy(&message, buff, sizeof(message));
	message.opcode = buff[0];
	message.length = buff[1] << 8 | buff[2];
	str_cpy(message.payload, buff + 3);
	return message;
}


int messageToBuff(Message message, char *buff) {
	buff[0] = (char)message.opcode;
	buff[1] = message.length >> 8;
	buff[2] = message.length & 0xFF;
	//Convert payload
	str_cpy(buff + 3, message.payload);
	return message.length + 3;
}

void readUserFromFile() {
	ifstream inFile;
	char fileName[300];
	strcpy(fileName, appPath);
	addToPath(fileName, "account.txt");
	inFile.open(fileName);

	if (!inFile) {
		cout << "Unable to open file account.txt";
		exit(1);   // call system to stop
	}
	int userid, status;
	char username[256], password[256];
	while (inFile >> userid >> username >> password >> status) {
		userList.push_back(User(userid, username, password, status));
	}
	inFile.close();
}