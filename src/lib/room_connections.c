/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
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
