#pragma once

#include "stdafx.h"

#ifndef _SERVICE_H_
#define _SERVICE_H_

int loginService(SOCKET socket, Message message);
int logoutService(SOCKET socket, Message message);
int registerService(SOCKET socket, Message message);
int uploadFileService(SOCKET socket, Message message);
int downloadFileService(SOCKET socket, Message message);
int createFolderService(SOCKET socket, Message message);
int tranferMetaService(SOCKET socket);
int tranferFile(SOCKET socket, char *file_name);
#endif