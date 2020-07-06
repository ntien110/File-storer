#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")

#ifndef _CONNECTION_H_
#define _CONNECTION_H_

int initServer(int port, SOCKET &listenSocket, SOCKET *socks, WSAEVENT *events, DWORD &nEvents);
SOCKET acceptClientSocket(SOCKET &socket, WSANETWORKEVENTS sockEvent);
int communicateClientSocket(SOCKET &socket, WSANETWORKEVENTS sockEvent);
int closeClientSocket(SOCKET &socket, WSANETWORKEVENTS sockEvent);

#endif