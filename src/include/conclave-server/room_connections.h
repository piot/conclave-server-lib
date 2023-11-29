/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_CONNECTIONS_H
#define CONCLAVE_SERVER_ROOM_CONNECTIONS_H

#include <clog/clog.h>
#include <conclave-serialize/types.h>
#include <monotonic-time/monotonic_time.h>
#include <stdlib.h>
#include <stdbool.h>

struct ClvRoomConnection;
struct ClvUserSession;

typedef struct ClvRoomConnections {
    struct ClvRoomConnection* connections;
    size_t connectionCount;
    size_t capacityCount;
    Clog log;
} ClvRoomConnections;

void clvRoomConnectionsInit(ClvRoomConnections* self, size_t maxCount, Clog log);
void clvRoomConnectionsDestroy(ClvRoomConnections* self);
int clvRoomConnectionsFindConnection(ClvRoomConnections* self, const struct ClvUserSession* ownerOfConnection,
                                     struct ClvRoomConnection** outConnection);
struct ClvRoomConnection* clvRoomConnectionsFindConnectionFromIndex(ClvRoomConnections* self, uint8_t connectionIndex);
void clvRoomConnectionsDestroyConnection(ClvRoomConnections* self, ClvSerializeRoomConnectionIndex index);
struct ClvRoomConnection* clvRoomConnectionsFindConnectionWithMostKnowledge(ClvRoomConnections* self, const struct ClvRoomConnection* excludeConnection);
bool clvRoomConnectionsHaveMostLostConnectionToOwner(const ClvRoomConnections* self);
void clvRoomConnectionsUpdate(ClvRoomConnections* self, MonotonicTimeMs now);

#endif
