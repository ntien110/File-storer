#pragma once

#include "stdafx.h"
#include "winsock2.h"

#ifndef _HELPER_H_
#define _HELPER_H_

int sendMessage(SOCKET client, char *message, int length);
int receiveMessage(SOCKET client, char *message);

#endif