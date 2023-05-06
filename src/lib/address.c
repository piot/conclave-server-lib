/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <conclave-server/address.h>
#include <stdio.h>
#include <tiny-libc/tiny_libc.h>

#if TORNADO_OS_WINDOWS
#include <Ws2tcpip.h>
#endif

int clvAddressEqual(const ClvAddress* a, const ClvAddress* b)
{
    return (a->sin_addr.s_addr == b->sin_addr.s_addr && a->sin_port == b->sin_port);
}

const char* clvAddressToString(const ClvAddress* self, char* temp, size_t maxCount)
{
    const char* converted = inet_ntop(AF_INET, &self->sin_addr, temp, maxCount - 6);

    size_t len = tc_strlen(converted);

    sprintf(&temp[len], ":%d", self->sin_port);

    return temp;
}
