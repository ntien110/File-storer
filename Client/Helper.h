#pragma once

#include "stdafx.h"
#include "winsock2.h"

#ifndef _HELPER_H_
#define _HELPER_H_

int sendMessage(char *message, int length);
int receiveMessage(char *message);
void str_cpy(char *dest, char *src, int len);
Message buffToMessage(char *buff);
int messageToBuff(Message message, char *buff);

#endif