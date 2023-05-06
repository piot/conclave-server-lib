/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_USER_SESSIONS_H
#define CONCLAVE_SERVER_USER_SESSIONS_H

#include <conclave-server/address.h>
#include <stdlib.h>

struct ClvUserSession;
struct sockaddr_in;
struct FldInStream;
struct ClvUser;

typedef struct ClvUserSessions {
    struct ClvUserSession* userSessions;
    size_t userSessionCapacity;
    size_t userSessionCount;
} ClvUserSessions;

void clvUserSessionsInit(ClvUserSessions* self);
void clvUserSessionsDestroy(ClvUserSessions* self);
void clvUserSessionsReset(ClvUserSessions* self);
int clvUserSessionsCreate(ClvUserSessions* sessions, struct ClvUser* user, const ClvAddress* address,
                          struct ClvUserSession** outSession);
int clvUserSessionsReadAndFind(const ClvUserSessions* self, const ClvAddress* address, struct FldInStream* stream,
                               struct ClvUserSession** outSession);

#endif
