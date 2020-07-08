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
	strcpy(password, &message_recv.payload[i + 1]);

	readUserFromFile();
	for (i = 0; i < userList.size(); i++) {
		if (strcmp(username, userList[i].username) == 0) {
			status = false;
			payload_resp = "Ten tai khoan da ton tai, vui long dang ki lai!";
			ret = messageToBuff(Message(FAIL, strlen(payload_resp), payload_resp), sendBuff);
			ret = sendMessage(socket, sendBuff, ret);
			return ret;
		}
	}

	char fileName[300]; 
	int length = 0;
	strcpy(fileName, appPath);
	addToPath(fileName, "account.txt");
	strcpy(new_user, "\n");
	itoa(userList.size() + 1, &new_user[1], 10);
	length = strlen(new_user);
	strcpy(&new_user[length], " ");
	strcpy(&new_user[length + 1], username);
	length = strlen(new_user);
	strcpy(&new_user[length], " ");
	strcpy(&new_user[length + 1], password);
	length = strlen(new_user);
	strcpy(&new_user[length], " 1");

	ret = appendToFile(fileName, new_user, strlen(new_user));
	if (ret == 0) {
		payload_resp = "Dang ki tai khoan thanh cong!";
		char file_name[BUFF_SIZE], userid[8];
		itoa(userList.size() + 1, userid, 10);
		strcpy(file_name, appPath);
		addToPath(file_name, userid);
		createDirectoryByPath(file_name);
		addToPath(file_name, "metaData");
		char *meta = "/";
		writeToFile(file_name, meta, 1);
		ret = messageToBuff(Message(SUCCESS, strlen(payload_resp), payload_resp), sendBuff);
	}
	else {
		payload_resp = "Dang ki tai khoan that bai, vui long thu lai!";
		ret = messageToBuff(Message(FAIL, strlen(payload_resp), payload_resp), sendBuff);
	}
	ret = sendMessage(socket, sendBuff, ret);
	return ret;
};

int loginService(SOCKET socket, Message message_recv) {
	Message message_resp;
	char username[256], password[256], sendBuff[BUFF_SIZE];
	char *payload_resp;
	int i, ret;
	//copy data from message's payload to usernaem, password ( separate by space(' '))
	for (i = 0; i < message_recv.length; i++) {
		if (message_recv.payload[i] == ' ') {
			username[i] = 0;
			break;
		}
		username[i] = message_recv.payload[i];
	}
	strcpy(password, &message_recv.payload[i + 1]);
	//logic login service: read file -> compare -> response
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
					userLogInAttemp.erase(socket);
				}
			}
			else {
				// wrong password
				if (userLogInAttemp.find(socket) == userLogInAttemp.end())
					userLogInAttemp.insert(std::pair<int, int>(socket, 1));
				else if (userLogInAttemp[socket] == 2) {
					userList[i].status = 0;
					writeUserToFile();
				} else
					userLogInAttemp[socket] = userLogInAttemp[socket] + 1;
				payload_resp = "Mat khau cua ban khong dung!";
			}
			break;
		}
		else
			payload_resp = "Khong tim thay ten dang nhap!";
	}

	if (strcmp(payload_resp, "Dang nhap thanh cong!") == 0)
		ret = messageToBuff(Message(SUCCESS, strlen(payload_resp), payload_resp), sendBuff);
	else
		ret = messageToBuff(Message(FAIL, strlen(payload_resp), payload_resp), sendBuff);
	ret = sendMessage(socket, sendBuff, ret);
	return ret;
};

int logoutService(SOCKET socket, Message message) {
	int ret;
	char sendBuff[BUFF_SIZE];
	if (userLogged[socket] != 0) {
		userLogged.erase(socket);
		char * payload_resp = "Dang xuat thanh cong!";
		ret = messageToBuff(Message(SUCCESS, strlen(payload_resp) , payload_resp), sendBuff);
	}
	else {
		char * payload_resp = "Ban chua dang nhap tai khoan nao!";
		ret = messageToBuff(Message(FAIL, strlen(payload_resp), payload_resp), sendBuff);
	}
	ret = sendMessage(socket, sendBuff, ret);
	return ret;
};

int uploadFileService(SOCKET socket, Message message) {
	int ret, result, index = 0;
	char recvBuff[BUFF_SIZE], sendBuff[BUFF_SIZE];
	Message message_resp;
	// payload parse
	message.payload[message.length] = '\n';
	message.payload[message.length+1] = '\0';

	vector<char*> components = split(message.payload, "\n");
	vector<char*> tracePath = split(components[0], ",");
	strcat(components[1], "\\");
	vector<char*> filePath = split(components[1], "\\");

	// get directory
	char file_name[BUFF_SIZE] = "", metadata[10000];
	char*  userid = new char[8];
	itoa(userLogged[socket], userid, 10);
	strcpy(file_name, appPath);
	addToPath(file_name, userid);
	addToPath(file_name, "metaData");

	result = readFile(file_name, 0, 10000, metadata);
	if (result < 0) {
		return -1;
	}
	Node* directoryTree = stringToTree(metadata, userid);

	// process update tree
	Node* curNode= directoryTree;
	for (int i = 0; i < tracePath.size(); i++) {
		curNode = curNode->children[i];
	}
	addNewNode(curNode, true, filePath[filePath.size() - 1], getTimestamp(), userid);
	curNode = curNode->children[curNode->children.size() - 1];
	do {
		result = receiveMessage(socket, recvBuff);
		while (result < 0) {
			result = receiveMessage(socket, recvBuff);
		}
		message_resp = buffToMessage(recvBuff);
		if (message_resp.opcode == TRANFER_DONE) break;
		//append message.payload to file
		appendToFile(curNode->getPath(), message_resp.payload, message_resp.length);
	} while (true);

	updateMetadata(directoryTree, userid);

	char * payload_resp = "Da tai file len thanh cong!";
	ret = messageToBuff(Message(SUCCESS, strlen(payload_resp), payload_resp), sendBuff);
	ret = sendMessage(socket, sendBuff, ret);
	return 0;
};

int downloadFileService(SOCKET socket, Message message) {
	return 0;
};

int tranferMetaService(SOCKET socket) {
	char userid[8], file_name[BUFF_SIZE] = "";
	itoa(userLogged[socket], userid, 10);
	strcpy(file_name, appPath);
	addToPath(file_name, userid);
	addToPath(file_name, "metaData");
	int ret = tranferFile(socket, file_name);
	return ret;
}

int tranferFile(SOCKET socket, char *file_name) {
	char sendBuff[BUFF_SIZE];
	char data[BUFF_SIZE];
	int ret = 0, index = 0;
	do {
		ret = readFile(file_name, index , 1024, data);
		data[ret] = 0;
		if (ret == -1) return -1;
		if (ret == 0) break;
		index += ret;
		int r = messageToBuff(Message(TRANFERING, ret, data), sendBuff);
		r = sendMessage(socket, sendBuff, r);
		if (r == -1) return -1;
	} while (ret != 0);
	ret = messageToBuff(Message(TRANFER_DONE, 4, "Done"), sendBuff);
	ret = sendMessage(socket, sendBuff, ret);
	return index;
}