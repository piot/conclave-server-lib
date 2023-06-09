/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_H
#define CONCLAVE_SERVER_ROOM_H

#include <clog/clog.h>
#include <conclave-server/room_connections.h>

struct ClvUser;
struct ClvUserSession;
struct ClvRoomConnection;
struct ImprintAllocator;

#include <conclave-serialize/types.h>
#include <stdlib.h>

typedef struct ClvRoom {
    ClvSerializeRoomId id;
    const char* name;
    const char* passcode;
    const struct ClvUser* ownedByUser;
    ClvRoomConnections roomConnections;
    Clog log;
    char prefix[16];
} ClvRoom;

void clvRoomInit(ClvRoom* self, size_t roomId, const char* roomName, const struct ClvUser* ownedByUser,
                 size_t maxMemberCount, Clog log);
int clvRoomCreateRoomConnection(ClvRoom* self, const struct ClvUserSession* foundUserSession,
                                struct ClvRoomConnection** outConnection);
struct ClvRoomConnection* clvRoomFindConnection(ClvRoom* self, uint8_t connectionIndex);
void clvRoomDebugOutput(const ClvRoom* self);
void clvRoomDestroy(ClvRoom* self);

#endif
