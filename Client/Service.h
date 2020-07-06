#pragma once

#include "stdafx.h"
#include "winsock2.h"

#ifndef _SERVICE_H_
#define _SERVICE_H_

Message loginService(SOCKET clientSocket, char *username, char *password);
Message logoutService(SOCKET clientSocket);
Message registerService(SOCKET clientSocket, char *username, char *password);
Message uploadFileService(SOCKET clientSocket, char *fileName);
Message downloadFileService(SOCKET clientSocket, char *fileName);

#endif