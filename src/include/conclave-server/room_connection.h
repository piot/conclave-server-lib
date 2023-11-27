/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_CONNECTION_H
#define CONCLAVE_SERVER_ROOM_CONNECTION_H

#include <conclave-serialize/types.h>
#include <conclave-server/connection_quality.h>
#include <stddef.h>
#include <stdint.h>

struct ClvUserSession;
struct ImprintAllocator;
struct ClvRoom;

typedef enum ClvRoomConnectionState {
    ClvRoomConnectionStateNormal,
    ClvRoomConnectionStateWaitingForReconnect,
    ClvRoomConnectionStateDisconnected
} ClvRoomConnectionState;

typedef struct ClvRoomConnection {
    ClvSerializeRoomConnectionIndex id;
    const struct ClvUserSession* owner;
    struct ClvRoom* ownedByRoom;
    ClvConnectionQuality connectionQuality;
    ClvSerializeKnowledge knowledge;
    ClvRoomConnectionState state;
} ClvRoomConnection;

void clvRoomConnectionInit(ClvRoomConnection* self, struct ClvRoom* ownedByRoom,
                           const struct ClvUserSession* userSession, MonotonicTimeMs now);

void clvRoomConnectionOnPing(ClvRoomConnection* self, ClvSerializeKnowledge knowledge, MonotonicTimeMs now);
bool clvRoomConnectionShouldDisconnect(const ClvRoomConnection* self);

#endif
