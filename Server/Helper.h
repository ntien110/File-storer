#pragma once

#include "stdafx.h"
#include "winsock2.h"

#ifndef _HELPER_H_
#define _HELPER_H_

int sendMessage(SOCKET client, char *message, int length);
int receiveMessage(SOCKET client, char *message);
void str_cpy(char *dest, char *src, int len);
Message buffToMessage(char *buff);
int messageToBuff(Message message, char *buff);
void readUserFromFile();
void writeUserToFile();
char* getTimestamp();

#endif