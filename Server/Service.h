#pragma once

#include "stdafx.h"

#ifndef _SERVICE_H_
#define _SERVICE_H_

int loginService(Message message);
int logoutService(Message message);
int registerService(Message message);
int uploadFileService(Message message);
int downloadFileService(Message message);

#endif