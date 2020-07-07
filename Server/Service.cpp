#include "stdafx.h"
#include "Service.h"
#include "Helper.h"
#include "FileManagement.h"

int registerService(SOCKET socket, Message message_recv) {
	Message message_resp;
	char username[256], password[256], sendBuff[BUFF_SIZE], new_user[BUFF_SIZE];
	char *payload_resp;
	int i, ret;
	bool status = true;

	for (i = 0; i < message_recv.length; i++) {
		if (message_recv.payload[i] == ' ') {
			username[i] = 0;
			break;
		}
		username[i] = message_recv.payload[i];
	}
	str_cpy(password, &message_recv.payload[i + 1]);

	readUserFromFile();
	for (i = 0; i < userList.size(); i++) {
		if (strcmp(username, userList[i].username) == 0) {
			status = false;
			payload_resp = "Ten tai khoan da ton tai, vui long dang ki lai";
			ret = messageToBuff(Message(REGISTER_FAIL, strlen(payload_resp), payload_resp), sendBuff);
			ret = sendMessage(socket, sendBuff, ret);
			return ret;
		}
	}

	char fileName[300]; 
	int length = 0;
	str_cpy(fileName, appPath);
	addToPath(fileName, "account.txt");
	str_cpy(new_user, "\n");
	//itoa(userList.size() + 1, &new_user[1], 10);
	itoa(userList.size() + 100, &new_user[1], 10);
	length = strlen(new_user);
	str_cpy(&new_user[length], " ");
	str_cpy(&new_user[length + 1], username);
	length = strlen(new_user);
	str_cpy(&new_user[length], " ");
	str_cpy(&new_user[length + 1], password);
	length = strlen(new_user);
	str_cpy(&new_user[length], " 1");

	ret = appendToFile(fileName, new_user, strlen(new_user));
	if (ret == 0) {
		payload_resp = "Dang ki tai khoan thanh cong!";
		ret = messageToBuff(Message(REGISTER_SUCCESS, strlen(payload_resp), payload_resp), sendBuff);
	}
	else {
		payload_resp = "Dang ki tai khoan that bai, vui long thu lai!";
		ret = messageToBuff(Message(REGISTER_FAIL, strlen(payload_resp), payload_resp), sendBuff);
	}
	ret = sendMessage(socket, sendBuff, ret);
	return ret;
};

int loginService(SOCKET socket, Message message_recv) {
	Message message_resp;
	char username[256], password[256], sendBuff[BUFF_SIZE];
	char *payload_resp;
	int i, ret;

	for (i = 0; i < message_recv.length; i++) {
		if (message_recv.payload[i] == ' ') {
			username[i] = 0;
			break;
		}
		username[i] = message_recv.payload[i];
	}
	str_cpy(password, &message_recv.payload[i + 1]);

	readUserFromFile();
	for (i = 0; i < userList.size(); i++) {
		if (strcmp(username, userList[i].username) == 0) {
			// account got locked
			if (userList[i].status == 0) {
				payload_resp = "Tai khoan cua ban da bi khoa!";
				break;
			}
			// right password
			if (strcmp(password, userList[i].password) == 0) {
				bool check = false;
				for (map<int, int>::iterator it = userLogged.begin(); it != userLogged.end(); ++it) {
					if (it->second == userList[i].userid) {
						check = true;
						break;
					}
				}
				if (check)
					payload_resp = "Tai khoan cua ban da dang nhap o client khac!";
				else {
					payload_resp = "Dang nhap thanh cong!";
					userLogged.insert(std::pair<int, int>(socket, userList[i].userid));
				}
			}
			else {
				payload_resp = "Mat khau cua ban khong dung!";
				// wrong password
				/*user[i].failedAttemp++;
				if (database[i].failedAttemp == 3) {
					database[i].status = 1;
					return 2;
				}*/
			}
			break;
		}
		else
			payload_resp = "Khong tim thay ten dang nhap!";
	}

	if (strcmp(payload_resp, "Dang nhap thanh cong!") == 0)
		ret = messageToBuff(Message(LOGIN_SUCCESS, strlen(payload_resp), payload_resp), sendBuff);
	else
		ret = messageToBuff(Message(LOGIN_FAIL, strlen(payload_resp), payload_resp), sendBuff);
	ret = sendMessage(socket, sendBuff, ret);
	return ret;
};

int logoutService(SOCKET socket, Message message) {
	int ret;
	char sendBuff[BUFF_SIZE];
	if (userLogged[socket] != 0) {
		userLogged.erase(socket);
		char * payload_resp = "Dang xuat thanh cong!";
		ret = messageToBuff(Message(LOGOUT_SUCCESS, strlen(payload_resp) , payload_resp), sendBuff);
	}
	else {
		char * payload_resp = "Ban chua dang nhap tai khoan nao!";
		ret = messageToBuff(Message(LOGOUT_FAIL, strlen(payload_resp), payload_resp), sendBuff);
	}
	ret = sendMessage(socket, sendBuff, ret);
	return ret;
};

int uploadFileService(SOCKET socket, Message message) {
	return 0;
};

int downloadFileService(SOCKET socket, Message message) {
	return 0;
};