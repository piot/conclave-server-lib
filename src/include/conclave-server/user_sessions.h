/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_USER_SESSIONS_H
#define CONCLAVE_SERVER_USER_SESSIONS_H

#include <stdlib.h>

struct ClvUserSession;
struct sockaddr_in;
struct FldInStream;
struct ClvUser;
struct GuiseSclUserSession;

#include <guise-sessions-client/address.h>

typedef struct ClvUserSessions {
    struct ClvUserSession* userSessions;
    size_t userSessionCapacity;
    size_t userSessionCount;
    Clog log;
    char prefix[32];
} ClvUserSessions;

void clvUserSessionsInit(ClvUserSessions* self, Clog log);
void clvUserSessionsDestroy(ClvUserSessions* self);
void clvUserSessionsReset(ClvUserSessions* self);
int clvUserSessionsCreate(ClvUserSessions* sessions, const struct GuiseSclUserSession* guiseUserSession,
                          struct ClvUserSession** outSession);
int clvUserSessionsReadAndFind(const ClvUserSessions* self, const GuiseSclAddress* address, struct FldInStream* stream,
                               const struct ClvUserSession** outSession);

#endif
