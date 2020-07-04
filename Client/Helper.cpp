#include "stdafx.h"
#include "helper.h"
#include <string.h>

#pragma comment (lib, "Ws2_32.lib")

#define BUFF_SIZE 8

using namespace std;

int sendMessage(SOCKET client, char *message, int length) {
	int ret, index = 0;
	char len[BUFF_SIZE];
	_itoa_s(length, len, 10);
	ret = send(client, len, BUFF_SIZE, 0);
	if (ret == SOCKET_ERROR) return SOCKET_ERROR;
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
	cout << "Da nhan do dai: " << length << endl;
	while (length > 0) {
		ret = recv(client, &message[index], length, 0);
		if (ret == SOCKET_ERROR) {
			cout << "Loi" << endl;
			continue;
		}
		index += ret;
		length -= ret;
	}
	message[index] = '\0';
	cout << "Da nhan: " << message << endl;
	return index;
}