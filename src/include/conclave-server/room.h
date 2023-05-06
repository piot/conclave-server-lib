/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_H
#define CONCLAVE_SERVER_ROOM_H

#include <conclave-server/room_connections.h>

struct ClvUser;
struct ClvUserSession;
struct ClvRoomConnection;
struct ImprintAllocator;

#include <stdlib.h>

typedef struct ClvRoom {
    uint64_t id;
    const char* name;
    const char* passcode;
    struct ClvUser* ownedByUser;
    ClvRoomConnections roomConnections;
    struct ImprintAllocator* allocator;
} ClvRoom;

int clvRoomCreateRoomConnection(ClvRoom* self, const struct ClvUserSession* foundUserSession,
                                struct ClvRoomConnection** outConnection);
void clvRoomDebugOutput(const ClvRoom* self);
void clvRoomDestroy(ClvRoom* self);

#endif
