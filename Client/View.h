#pragma once
#include "stdafx.h"
#include "winsock2.h"
#include <string>
#include <iomanip>

using namespace std;

#ifndef _VIEW_H_
#define _VIEW_H_

void drawOptions(string title, string options[], int count);
void drawResponse(int code, string response);

void registerView();
void loginView();
void logoutView();
void workWithFolder();

#endif