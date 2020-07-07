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

void registerView() {
	char username[256], password[256];
	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	cin >> password;

	Message result = registerService(username, password);
	drawResponse(result.opcode, result.payload);
	return;
};

void loginView(bool &isLoggedIn) {
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
	if (result.opcode == LOGIN_SUCCESS) isLoggedIn = true;
	drawResponse(result.opcode, result.payload);
	return;
};

void logoutView(bool &isLoggedIn) {
	if (!isLoggedIn) {
		drawResponse(0, "Ban chua dang nhap, khong the dang xuat!");
		return;
	}
	Message result = logoutService();
	if (result.opcode == NOT_FOUND) {
		drawResponse(0, "Khong the xu li yeu cau, vui long thu lai!");
		return;
	}
	else {
		if (result.opcode == LOGOUT_SUCCESS) isLoggedIn = false;
		drawResponse(result.opcode, result.payload);
		return;
	}
};

void uploadFileView(bool &isLoggedIn) {};

void downloadFileView(bool &isLoggedIn) {};