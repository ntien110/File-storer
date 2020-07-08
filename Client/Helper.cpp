#include "stdafx.h"
#include "helper.h"
#include <string.h>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int sendMessage(char *message, int length) {
	int ret, index = 0;
	char len[BUFF_SIZE];
	_itoa_s(length, len, 10);
	ret = send(CLIENT, len, BUFF_SIZE, 0);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	cout << "Da gui do dai: " << length << endl;
	while (length > 0) {
		ret = send(CLIENT, &message[index], length, 0);
		if (ret == SOCKET_ERROR) continue;
		index += ret;
		length -= ret;
	}
	cout << "Da gui ret: " << ret << endl;
	return index;
}

//length do dai du lieu muon nhan
int receiveMessage(char *message) {
	char len[BUFF_SIZE];
	int ret, index = 0;
	ret = recv(CLIENT, len, BUFF_SIZE, 0);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
	int length = atoi(len);
	while (length > 0) {
		ret = recv(CLIENT, &message[index], length, 0);
		if (ret == SOCKET_ERROR) {
			continue;
		}
		index += ret;
		length -= ret;
	}
	cout << "Da nhan: " << endl;
	message[index] = '\0';
	return index;
}

void str_cpy(char *dest, char *src, int len) {
	for (int i = 0; i < len; i++) {
		dest[i] = src[i];
	}
	//dest[len] = '/0';
}

Message buffToMessage(char *buff) {
	Message message;
	message.opcode = buff[0];
	message.length = (uint8_t) buff[1] << 8 | (uint8_t) buff[2];
	str_cpy(message.payload, buff + 3, message.length);
	message.payload[message.length] = 0;
	return message;
}


int messageToBuff(Message message, char *buff) {
	buff[0] = (char)message.opcode;
	buff[1] = message.length >> 8;
	buff[2] = message.length & 0xFF;
	//Convert payload
	str_cpy(buff + 3, message.payload, message.length);
	buff[message.length + 3] = 0;
	return message.length + 3;
}