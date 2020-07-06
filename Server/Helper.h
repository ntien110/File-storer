#pragma once

#include "stdafx.h"
#include "winsock2.h"

#ifndef _HELPER_H_
#define _HELPER_H_

int sendMessage(SOCKET client, char *message, int length);
int receiveMessage(SOCKET client, char *message);
void str_cpy(char *dest, char *src);
Message buffToMessage(char *buff);
void messageToBuff(Message message, char *buff);

#endif