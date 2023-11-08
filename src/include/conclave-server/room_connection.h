/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_CONNECTION_H
#define CONCLAVE_SERVER_ROOM_CONNECTION_H

#include <conclave-serialize/types.h>
#include <stddef.h>
#include <stdint.h>

struct ClvUserSession;
struct ImprintAllocator;
struct ClvRoom;

typedef struct ClvRoomConnection {
    ClvSerializeRoomConnectionIndex id;
    const struct ClvUserSession* owner;
    struct ClvRoom* ownedByRoom;
} ClvRoomConnection;

void clvRoomConnectionInit(ClvRoomConnection* self, struct ClvRoom* ownedByRoom,
                           const struct ClvUserSession* userSession);

#endif
