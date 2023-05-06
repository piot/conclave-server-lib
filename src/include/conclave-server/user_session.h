/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_USER_SESSION_H
#define CONCLAVE_SERVER_USER_SESSION_H

#include <conclave-server/address.h>

struct ClvUser;

typedef struct ClvUserSession {
    ClvAddress address;
    struct ClvUser* user;
} ClvUserSession;

#endif
