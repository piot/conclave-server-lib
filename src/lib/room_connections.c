/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <conclave-server/room_connection.h>
#include <conclave-server/room_connections.h>
#include <tiny-libc/tiny_libc.h>

void clvRoomConnectionsInit(ClvRoomConnections* self, size_t maxCount)
{
    self->connectionCount = 0;
    self->connections = tc_malloc_type_count(ClvRoomConnection, maxCount);
    self->capacityCount = maxCount;

    tc_mem_clear_type_n(self->connections, self->capacityCount);
}

void clvRoomConnectionsDestroy(ClvRoomConnections* self)
{
    tc_free(self->connections);
}

void clvRoomConnectionsDestroyConnection(ClvRoomConnections* self, ClvRoomConnection* connection)
{
    (void) self;
    connection->ownedByRoom = 0;
    connection->owner = 0;
}

int clvRoomConnectionsFindConnection(ClvRoomConnections* self, const struct ClvUserSession* ownerOfConnection,
                                     ClvRoomConnection** outConnection)
{
    for (size_t i = 0; i < self->connectionCount; ++i) {
        ClvRoomConnection* roomConnection = &self->connections[i];
        if (roomConnection->owner == ownerOfConnection) {
            *outConnection = roomConnection;

            return 1;
        }
    }

    *outConnection = 0;

    return 0;
}

ClvRoomConnection* clvRoomConnectionsFindConnectionWithMostKnowledge(ClvRoomConnections* self)
{
    ClvRoomConnection* bestConnection = 0;

    for (size_t i = 0; i < self->connectionCount; ++i) {
        ClvRoomConnection* roomConnection = &self->connections[i];
        if (roomConnection->owner == 0) {
            continue;
        }
        if (bestConnection == 0 || roomConnection->knowledge > bestConnection->knowledge) {
            bestConnection = roomConnection;
        }
    }

    return bestConnection;
}
