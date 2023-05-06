/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_CONNECTION_H
#define CONCLAVE_SERVER_ROOM_CONNECTION_H

#include <stddef.h>
#include <stdint.h>

struct ClvUserSession;
struct ImprintAllocator;
struct ClvRoom;

/** Represents a unreliable datagram transport "connection" from a client */
typedef struct ClvRoomConnection {
    uint32_t id;
    const struct ClvUserSession* owner;
    struct ClvRoom* ownedByRoom;
} ClvRoomConnection;

void clvRoomConnectionInit(ClvRoomConnection* self, struct ClvRoom* ownedByRoom,
                           const struct ClvUserSession* userSession);

#endif
