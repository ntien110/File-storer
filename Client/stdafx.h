// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>

using namespace std;

#define BUFF_SIZE 2048
#define BUFF_FILE 8192

struct Message {
	uint8_t opcode;
	uint16_t length;
	char payload[2016];
	Message() {};
	Message(uint8_t _opcode, uint16_t _length, char *_payload) {
		opcode = _opcode;
		length = _length;
		strcpy_s(payload, sizeof payload, _payload);
	}
};

enum REQUEST_CODE {
	REGISTER = 100,
	LOGIN = 110,
	LOGOUT = 120,
	UPLOAD_FILE = 130,
	DOWNLOAD_FILE = 140
};

enum RESPONSE_CODE {
	REGISTER_SUCCESS = 200,
	REGISTER_FAIL = 204,

	LOGIN_SUCCESS = 210,
	LOGIN_FAIL = 214,

	LOGOUT_SUCCESS = 220,
	LOGOUT_FAIL = 224,

	UPLOAD_FILE_SUCCESS = 230,
	UPLOAD_FILE_FAIL = 234,

	DOWNLOAD_FILE_SUCCESS = 240,
	DOWNLOAD_FILE_FAIL = 244,
};
// TODO: reference additional headers your program requires here
