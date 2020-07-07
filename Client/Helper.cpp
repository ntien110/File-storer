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
	while (length > 0) {
		ret = send(CLIENT, &message[index], length, 0);
		if (ret == SOCKET_ERROR) continue;
		index += ret;
		length -= ret;
	}
	cout << "Da gui: " << message << endl;
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
	//memcpy(buff, &message, sizeof(message));
	buff[0] = (char)message.opcode;
	buff[1] = message.length >> 8;
	buff[2] = message.length & 0xFF;
	//Convert payload
	str_cpy(buff + 3, message.payload);
	return message.length + 3;
}