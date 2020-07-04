#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

int initServer(int port, SOCKET &listenSocket);
int initAcceptEvent(WSAEVENT &AcceptEvent);
int acceptFromClient(SOCKET &listenSocket, SOCKET &clientSocket, WSAEVENT &acceptEvent);

#endif