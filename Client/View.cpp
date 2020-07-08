#include "stdafx.h"
#include "View.h"
#include "Service.h"
#include "Helper.h"
#include "FileManagement.h"
#include <stack>
#include <conio.h>

#pragma warning(disable:4996)

stack<Node*> folderStack;
vector<int> pathStack;
Node* curNode;

void resetDirectoryTree() {
	char* metadata = new char[8296];
	int result = getMetadataService(metadata);
	if (result <= 0) {
		cout << "Error on get user directory" << endl;
		getch();
		metadata = "//";
	}
	curNode = stringToTree(metadata, userid);

	while (folderStack.size() > 0) {
		folderStack.pop();
	}
	folderStack.push(curNode);

	while (pathStack.size() > 0) {
		pathStack.pop_back();
	}
}

char* getCurTracePath() {
	char* tracePath = new char[64];
	strcpy(tracePath, ",");
	char temp[3];
	for (int i = 0; i < pathStack.size(); i++) {
		itoa(pathStack[i], temp, 10);
		strcat(tracePath, temp);
		strcat(tracePath, ",");
	}
	return tracePath;
}


void drawOptions(string title, string options[], int count) {
	cout << "\t-------------------------------\t" << setw(10) << left << title << "---------------------------------\n";
	for (int i = 0; i < count; i++) {
		cout << setw(76) << left << "\t|" << "|\n";
		cout << "\t| " << i + 1 << ". " << setw(70) << left << options[i] << "|\n";
		cout << setw(76) << left << "\t|" << "|\n";
	}
	cout << "\t----------------------------------------------------------------------------\n";
}

void drawResponse(int code, string response) {
	cout << "\t--------------------------------" << "PHAN HOI" << "-----------------------------------\n";
	cout << setw(76) << left << "\t|" << "|\n";
	cout << "\t| " << code << ": " << setw(68) << left << response << "|\n";
	cout << setw(76) << left << "\t|" << "|\n";
	cout << "\t----------------------------------------------------------------------------\n";
}

void drawMessage(string message) {
	cout << "\t--------------------------------" << "THONG BAO" << "-----------------------------------\n";
	cout << setw(76) << left << "\t|" << "|\n";
	cout << "\t| " << setw(68) << left << message << "|\n";
	cout << setw(76) << left << "\t|" << "|\n";
	cout << "\t----------------------------------------------------------------------------\n";
}

void drawFileStructure() {

	cout << "\t--------------------------" << "NOI DUNG THU MUC: " << curNode->name << "------------------------------\n";

	for (int i = 0; i < curNode->children.size(); i++) {
		string temp = "  ";
		if (curNode->children[i]->isFile) {
			temp = "* ";
		}
		cout << setw(76) << left << "\t|" << "|\n";
		cout << "\t| " << temp << setw(71) << left << curNode->children[i]->name << "|\n";
		cout << setw(76) << left << "\t|" << "|\n";
	}
	cout << "\t----------------------------------------------------------------------------\n";
}

void registerView() {
	char username[256], password[256];
	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	cin >> password;

	Message result = registerService(username, password);
	if (result.opcode == NULL) {
		drawResponse(NOT_FOUND, "Khong the thuc hien hanh dong nay, vui long thu lai");
	}
	else
		drawResponse(result.opcode, result.payload);
	return;
};

void loginView() {
	char username[256], password[256];

	if (isLoggedIn) {
		drawResponse(0, "Ban da dang nhap, vui long dang xuat!");
		return;
	}
	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	cin >> password;
	Message result = loginService(username, password);
	if (result.opcode == NULL) {
		result = Message(NOT_FOUND, 52, "Khong the thuc hien hanh dong nay, vui long thu lai!");
	}
	else if (result.opcode == SUCCESS) {
		isLoggedIn = true; //set status of current client is logged
		str_cpy(userid, result.payload, result.length); //set userid of current client
		strcpy(result.payload, "Dang nhap thanh cong!");
	}
	drawResponse(result.opcode, result.payload);
	return;
};

void logoutView() {
	if (!isLoggedIn) {
		drawResponse(0, "Ban chua dang nhap, khong the dang xuat!");
		return;
	}
	Message result = logoutService();
	if (result.opcode == NULL) {
		result = Message(NOT_FOUND, 52, "Khong the thuc hien hanh dong nay, vui long thu lai!");
	}
	else if (result.opcode == SUCCESS) {
		isLoggedIn = false; //set status of current client is logged
		strcpy(userid, ""); //set userid of current client
		strcpy(result.payload, "Dang xuat thanh cong!");
	}
	drawResponse(result.opcode, result.payload);
	return;
};

Node* selectView(bool folderOnly) {
	vector<Node*> nodes;
	for (int i = 0; i < curNode->children.size(); i++) {
		if (!curNode->children[i]->isFile || !folderOnly) {
			nodes.push_back(curNode->children[i]);
		}
	}

	char chosen[2];
	do {
		system("CLS");
		cout << "\t-------------------------------\t" << setw(10) << left << "OPTION" << "---------------------------------\n";
		for (int i = 0; i < nodes.size(); i++) {
			string temp = "  ";
			if (nodes[i]->isFile) {
				temp = "* ";
			}
			cout << setw(76) << left << "\t|" << "|\n";
			cout << "\t| " << i + 1 << ". " << temp << setw(68) << left << nodes[i]->name << "|\n";
			cout << setw(76) << left << "\t|" << "|\n";
		}
		cout << "\t--------------------------------ACTIONS-------------------------------------\n";
		cout << setw(76) << left << "\t|" << "|\n";
		cout << "\t| " << nodes.size() + 1 << ". " << setw(70) << left << "  Quay lai" << "|\n";
		cout << setw(76) << left << "\t|" << "|\n";

		cout << "\t----------------------------------------------------------------------------\n";
		cout << "Nhap lua chon cua ban: ";
		cin >> chosen;
	} while (atoi(chosen) <= 0 || atoi(chosen) > nodes.size() + 1);

	int t = atoi(chosen);

	system("CLS");
	if (t == nodes.size() + 1) {
		return NULL;
	}
	if (t > 0 && t <= nodes.size()) {
		return nodes[t - 1];
	}
}

void goToView() {
	folderStack.push(curNode);
	curNode = selectView(true);

	if (!curNode) {
		curNode = folderStack.top();
		folderStack.pop();
	}
	else {
		for (int i = 0; i < folderStack.top()->children.size(); i++) {
			if (strcmp(folderStack.top()->children[i]->name, curNode->name) == 0) {
				pathStack.push_back(i);
				break;
			}
		}
	}
}

void createFolderView() {
	char nameFolder[256];

	cout << "Nhap ten folder muon tao: ";
	cin >> nameFolder;
	Message result = createFolderService(getCurTracePath(), nameFolder);
	if (result.opcode == NULL) {
		result = Message(NOT_FOUND, 52, "Khong the thuc hien hanh dong nay, vui long thu lai!");
	}
	else {
		drawResponse(result.opcode, result.payload);//result.opcode, result.payload);
		resetDirectoryTree();
	}
	return;
}

void uploadFolderView() {
	char path[256];

	cout << "Path to directory: ";
	cin >> path;
	//Message result = loginService(username, password);
	drawResponse(111, path);//result.opcode, result.payload);
	resetDirectoryTree();
	return;
}

void uploadFileView() {
	char path[256];
	cout << "Nhap ten file muon tai: ";
	cin >> path;
	Message result = uploadFileService(getCurTracePath(), path);
	if (result.opcode == NULL) {
		result = Message(NOT_FOUND, 52, "Khong the thuc hien hanh dong nay, vui long thu lai!");
	}
	else {
		drawResponse(result.opcode, result.payload);//result.opcode, result.payload);
		resetDirectoryTree();
	}
	return;
}

void downloadView() {
	Node* selectedNode = selectView(false);
	if (!selectedNode) {
		return;
	}
	
	folderStack.push(curNode);

	for (int i = 0; i < folderStack.top()->children.size(); i++) {
		//cout << folderStack.top()->children[i]->name << ":" << selectedNode->name << endl;
		if (strcmp(folderStack.top()->children[i]->name, selectedNode->name) == 0) {
			pathStack.push_back(i);
			cout << "ok" << endl;
			break;
		}
	}
	char saveLocation[256];
	cout << "Path to save file/folder: ";
	cin >> saveLocation;

	Message result;
	if (selectedNode->isFile) {
		result = downloadFileService(getCurTracePath(), saveLocation);
	}
	else {
		result = downloadFolderService(getCurTracePath(), saveLocation);
	}

	if (result.opcode == NULL) {
		result = Message(NOT_FOUND, 52, "Khong the thuc hien hanh dong nay, vui long thu lai!");
	}
	drawResponse(result.opcode, result.payload);
	resetDirectoryTree();
	return;
}

void deleteView() {
	Node* selectedNode = selectView(false);
	if (!selectedNode) {
		return;
	}

	//Message result = loginService(username, password);
	drawResponse(111, "Deleting...");//result.opcode, result.payload);
	resetDirectoryTree();
	return;
}

void workWithFolder() {
	if (!isLoggedIn) {
		drawResponse(0, "Ban chua dang nhap, khong the thao tac voi thu muc!");
		return;
	}

	resetDirectoryTree();

	string options[8] = {
		"Di den",
		"Lui lai",
		"Them thu muc",
		"Tai thu muc len",
		"Tai tep len",
		"Tai xuong",
		"Xoa",
		"Huy"
	};
	char chosen;
	do {
		system("CLS");
		drawFileStructure();
		drawOptions("ACTION", options, 8);
		cout << "Nhap lua chon cua ban: ";
		cin >> chosen;
		switch (chosen) {
		case '1':
			goToView();
			break;
		case '2':
			if (!folderStack.empty()) {
				curNode = folderStack.top();
				folderStack.pop();
			}
			if (!pathStack.empty()) {
				pathStack.pop_back();
			}
			break;
		case '3':
			createFolderView();
			break;
		case '4':
			uploadFolderView();
			break;
		case '5':
			uploadFileView();
			break;
		case '6':
			downloadView();
			break;
		case '7':
			deleteView();
			break;
		default:
			drawResponse(0, "Lua chon khong phu hop, vui long chon lai!!!!");
			break;
		};
	} while (chosen != '8');

	system("CLS");
	return;
}