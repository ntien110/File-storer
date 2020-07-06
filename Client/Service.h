#pragma once

#include "stdafx.h"

#ifndef _SERVICE_H_
#define _SERVICE_H_

Message loginService(char *username, char *password);
Message logoutService();
Message registerService(char *username, char *password);
Message uploadFileService(char *fileName);
Message downloadFileService(char *fileName);

#endif