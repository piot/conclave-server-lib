/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_USER_SESSION_H
#define CONCLAVE_SERVER_USER_SESSION_H

#include <conclave-server/address.h>
#include <clog/clog.h>
#include <conclave-serialize/types.h>

struct ClvUser;

typedef struct ClvUserSession {
    ClvSerializeUserSessionId userSessionId;
    ClvAddress address;
    const struct ClvUser* user;
    Clog log;
    char prefix[32];
} ClvUserSession;

void clvUserSessionInit(ClvUserSession* self, ClvSerializeUserSessionId userSessionId, const ClvAddress* address, const struct ClvUser* owner, Clog log);

#endif
