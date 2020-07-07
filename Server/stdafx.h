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
#include <vector>
#include <map>

using namespace std;

#define BUFF_SIZE 2048
#define BUFF_FILE 8192

struct Message {
	uint8_t opcode;
	uint16_t length;
	char payload[BUFF_SIZE];
	Message() {};
	Message(uint8_t _opcode, uint16_t _length, char *_payload) {
		opcode = _opcode;
		length = _length;
		strcpy_s(payload, sizeof payload, _payload);
	}
};

struct User {
	int userid;
	char username[256];
	char password[256];
	int status;
	User() {};
	User(int _userid, char *_username, char *_password, int _status) {
		userid = _userid;
		strcpy_s(username, sizeof username, _username);
		strcpy_s(password, sizeof password, _password);
		status = _status;
	}
};
extern vector<User> userList;
extern map<int, int> userLogged;

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

	NOT_FOUND = 000,
};
// TODO: reference additional headers your program requires here
