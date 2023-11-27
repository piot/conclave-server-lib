/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_H
#define CONCLAVE_SERVER_ROOM_H

#include <clog/clog.h>
#include <conclave-server/room_connections.h>

struct ClvUserSession;
struct ClvRoomConnection;
struct ImprintAllocator;
struct GuiseSclUserSession;
struct ClvRoomConnection;

#include <conclave-serialize/types.h>
#include <stdlib.h>

struct ClvUserSessions;
struct GuiseSclUserSession;

typedef struct ClvRoom {
    size_t id;
    ClvSerializeApplicationId applicationId;
    ClvSerializeApplicationVersion applicationVersion;
    char name[64];
    const char* passcode;
    // const struct ClvUserSession* ownedByConclaveSession;
    ClvRoomConnections roomConnections;
    struct ClvRoomConnection* ownedByConnection;
    const struct GuiseSclUserSession* createdByUserSession;
    Clog log;
    char prefix[16];
} ClvRoom;

typedef struct ClvRoomConfig {
    const struct GuiseSclUserSession* createdByUserSession;
    ClvSerializeApplicationId applicationId;
    ClvSerializeApplicationVersion applicationVersion;
    size_t indexInRooms;
    const char* roomName;
    size_t maxMemberCount;
    Clog log;
} ClvRoomConfig;

void clvRoomInit(ClvRoom* self, const ClvRoomConfig* config);
int clvRoomCreateRoomConnection(ClvRoom* self, const struct ClvUserSession* foundUserSession, MonotonicTimeMs now,
                                struct ClvRoomConnection** outConnection);
void clvRoomCheckForDisconnections(struct ClvUserSessions* sessions, ClvRoom* self);
void clvRoomSelectNewOwner(ClvRoom* self);
void clvRoomDebugOutput(const ClvRoom* self);
void clvRoomDestroy(ClvRoom* self);

#endif
