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
#define PAYLOAD_SIZE BUFF_SIZE - 3

struct Message {
	uint8_t opcode;
	uint16_t length;
	char payload[PAYLOAD_SIZE];
	Message() {};
	Message(uint8_t _opcode, uint16_t _length, char *_payload) {
		cout << "dfdsf" << _length << endl;
		opcode = _opcode;
		length = _length;
		for (int i = 0; i < _length; i++) {
			payload[i] = _payload[i];
		}
		//str_cpy(payload, _payload, _length);
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
extern map<int, int> userLogInAttemp;

enum REQUEST_CODE {
	REGISTER = 100,
	LOGIN = 110,
	LOGOUT = 120,
	GET_META_FILE = 130,
	UPLOAD_FILE = 140,
	DOWNLOAD_FILE = 150,
	UPLOAD_FOLDER = 160,
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
// TODO: reference additional headers your program requires here
