// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include "winsock2.h"

#pragma warning(disable:4996)

using namespace std;

#define BUFF_SIZE 8192
#define BUFF_FILE 8000
#define PAYLOAD_SIZE BUFF_SIZE - 3

struct Message {
	uint8_t opcode;
	uint16_t length;
	char payload[PAYLOAD_SIZE];
	Message() {};
	Message(uint8_t _opcode, uint16_t _length, char *_payload) {
		opcode = _opcode;
		length = _length;
		for (int i = 0; i < _length; i++) {
			payload[i] = _payload[i];
		}
		//str_cpy(payload, _payload, _length);
	}
};

enum REQUEST_CODE {
	REGISTER = 100,
	LOGIN = 110,
	LOGOUT = 120,
	GET_META_FILE = 130,
	UPLOAD_FILE = 140,
	DOWNLOAD_FILE = 150,
	CREATE_FOLDER = 160,
	//UPLOAD_FOLDER = 160,
	DOWNLOAD_FOLDER = 170,
	DELETE_DATA = 180
};

enum RESPONSE_CODE {
	SUCCESS = 200,

	FAIL = 204,

	TRANFER_DONE = 251,
	TRANFERING = 252,
	NOT_FOUND = 000,
};

extern SOCKET CLIENT;
extern char userid[8];
extern bool isLoggedIn;
// TODO: reference additional headers your program requires here
