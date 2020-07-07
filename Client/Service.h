#pragma once

#include "stdafx.h"

#ifndef _SERVICE_H_
#define _SERVICE_H_

Message loginService(char *username, char *password);
Message logoutService();
Message registerService(char *username, char *password);
Message getMetaFolderService();
Message uploadFileService();
Message downloadFileService(char *fileName);
Message uploadFolderService();
Message downloadFolderService(char *fileName);
Message deleteFileService(char *pathTrace);

#endif