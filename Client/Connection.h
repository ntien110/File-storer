#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

SOCKET connectToServer(int port, char *ip_addr);

#endif