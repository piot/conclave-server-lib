/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ADDRESS_H
#define CONCLAVE_SERVER_ADDRESS_H

#if TORNADO_OS_WINDOWS
#include <WinSock2.h>
#include <Windows.h>
#else
#include <netinet/in.h>
#endif

typedef struct sockaddr_in ClvAddress;
int clvAddressEqual(const ClvAddress* a, const ClvAddress* b);
const char* clvAddressToString(const ClvAddress* self, char* temp, size_t maxCount);

#endif
