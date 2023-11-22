/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_CONNECTIONS_H
#define CONCLAVE_SERVER_ROOM_CONNECTIONS_H

#include <monotonic-time/monotonic_time.h>
#include <stdlib.h>

struct ClvRoomConnection;
struct ClvUserSession;

typedef struct ClvRoomConnections {
    struct ClvRoomConnection* connections;
    size_t connectionCount;
    size_t capacityCount;
} ClvRoomConnections;

void clvRoomConnectionsInit(ClvRoomConnections* self, size_t maxCount);
void clvRoomConnectionsDestroy(ClvRoomConnections* self);
int clvRoomConnectionsFindConnection(ClvRoomConnections* self, const struct ClvUserSession* ownerOfConnection,
                                     struct ClvRoomConnection** outConnection);
void clvRoomConnectionsDestroyConnection(ClvRoomConnections* self, struct ClvRoomConnection* connection);
struct ClvRoomConnection* clvRoomConnectionsFindConnectionWithMostKnowledge(ClvRoomConnections* self);

#endif
