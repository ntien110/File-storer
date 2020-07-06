// Client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "conio.h"
#include "helper.h"
#include "Connection.h"
#include <string>
#include "mswsock.h"
#include "View.h"
#include "Service.h"

#pragma comment(lib, "Mswsock.lib")

#define PORT 5500
#define IPADDR "127.0.0.1"

using namespace std;

int main(int argc, char *argv[]) {
	SOCKET client;
	char chosen;
	//Request connection to server
	client = connectToServer(PORT, IPADDR);
	if (client == NULL) return 0;
	//Step 5: Communicate with server
	bool isLoggedIn = false;
	string options[6] = { "Dang ki tai khoan", "Dang nhap", "Tai file len server", "Doc file tu server", "Dang xuat", "Thoat chuong trinh"};
	do {
		drawOptions("MENU", options, 6);
		cout << "Nhap lua chon cua ban: ";
		cin >> chosen;
		system("CLS");
		switch (chosen) {
		case '1':
			registerView(client);
			break;
		case '2':
			loginView(client, isLoggedIn);
			break;
		case '3':
			uploadFileView(client, isLoggedIn);
			break;
		case '4':
			downloadFileView(client, isLoggedIn);
			break;
		case '5':
			logoutView(client, isLoggedIn);
			break;
		case '6':
		{
			//Step 6: Close socket and Terminate Winsock
			closesocket(client);
			WSACleanup();
			break;
		}
		default:
			cout << "Lua chon khong phu hop, vui long chon lai!!!!/n";
			break;
		};
	} while (chosen != '6');
	return 0;
}