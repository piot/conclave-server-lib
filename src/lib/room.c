/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-server/room.h>
#include <conclave-server/room_connection.h>

static int roomCreateConnection(ClvRoom* self, const struct ClvUserSession* ownerOfConnection,
                                ClvRoomConnection** outConnection)
{
    ClvRoomConnections* connections = &self->roomConnections;

    for (size_t i = 0; i < connections->capacityCount; ++i) {
        ClvRoomConnection* roomConnection = &connections->connections[i];
        if (roomConnection->owner == 0) {
            roomConnection->owner = ownerOfConnection;
            if (ownerOfConnection == 0) {
                CLOG_C_ERROR(&self->log, "could not create a connection in the room, owner is null")
            }
            roomConnection->id = (ClvSerializeRoomConnectionIndex) i;
            clvRoomConnectionInit(roomConnection, self, ownerOfConnection);
            connections->connectionCount++;

            *outConnection = roomConnection;
            return 0;
        }
    }

    *outConnection = 0;

    return -1;
}

void clvRoomDestroy(ClvRoom* self)
{
    clvRoomConnectionsDestroy(&self->roomConnections);
}

ClvRoomConnection* clvRoomFindConnection(ClvRoom* self, uint8_t connectionIndex)
{
    if (connectionIndex >= self->roomConnections.connectionCount) {
        return 0;
    }

    return &self->roomConnections.connections[connectionIndex];
}

void clvRoomInit(ClvRoom* self, size_t indexInRooms, const char* roomName,
                 const struct ClvUserSession* requiredUserSession, size_t maxMemberCount, Clog log)
{
    self->log = log;
    self->name = tc_str_dup(roomName);
    self->id = indexInRooms;
    self->ownedByConclaveSession = requiredUserSession;
    clvRoomConnectionsInit(&self->roomConnections, maxMemberCount);
}

int clvRoomCreateRoomConnection(ClvRoom* self, const struct ClvUserSession* foundUserSession,
                                ClvRoomConnection** outConnection)
{
    if (foundUserSession == 0) {
        CLOG_C_ERROR(&self->log, "creator is null");
    }

    int errorCode = clvRoomConnectionsFindConnection(&self->roomConnections, foundUserSession, outConnection);
    if (errorCode < 0) {
        *outConnection = 0;
        return errorCode;
    }

    if (errorCode == 1) {
        return 0;
    }

    errorCode = roomCreateConnection(self, foundUserSession, outConnection);
    if (errorCode < 0) {
        *outConnection = 0;
        return errorCode;
    }

    return 0;
}

#if defined CLOG_LOG_ENABLED
void clvRoomDebugOutput(const ClvRoom* self)
{
    CLOG_C_INFO(&self->log, "'%s' (members %u, connections %zu)", self->name, 0u, self->roomConnections.connectionCount)
}

#endif
