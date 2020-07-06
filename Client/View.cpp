#include "stdafx.h"
#include "View.h"
#include "Service.h"
#include "Helper.h"

void drawOptions(string title, string options[], int count) {
	cout << "\t-------------------------------\t" << setw(10) << left << title << "---------------------------------\n";
	for (int i = 0; i < count; i++) {
		cout << setw(76) << left << "\t|" << "|\n";
		cout << "\t| " << i+1 << ". " << setw(70) << left << options[i] << "|\n";
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

void registerView(SOCKET clientSocket) {
	char username[256], password[256];
	registerService(clientSocket, "", "");
};

void loginView(SOCKET clientSocket, bool &isLoggedIn) {
	char username[256], password[256];

	if (isLoggedIn) {
		cout << "You have already logged in. Please logout first.\n\n";
		return;
	}
	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	cin >> password;
	Message result = loginService(clientSocket, username, password);
	if (result.opcode == NULL) {
		drawResponse(0, "Failed! wrong username or password");
		return;
	} else {
		drawResponse(result.opcode, result.payload);
		return;
	}
};

void logoutView(SOCKET clientSocket, bool &isLogged) {};
void uploadFileView(SOCKET clientSocket, bool &isLogged) {};
void downloadFileView(SOCKET clientSocket, bool &isLogged) {};