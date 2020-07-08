#pragma once

#include "stdafx.h"

#ifndef _SERVICE_H_
#define _SERVICE_H_

Message loginService(char *username, char *password);
Message logoutService();
Message registerService(char *username, char *password);
int getMetadataService(char *metaData);
Message uploadFileService(char *pathDir, char *fileName);
Message downloadFileService(char* tracePath, char* saveLocation);
Message uploadFolderService();
Message downloadFolderService(char* tracePath, char* saveLocation);
Message deleteFileService(char *pathTrace);

#endif