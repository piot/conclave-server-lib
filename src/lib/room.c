/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-server/room.h>
#include <conclave-server/room_connection.h>
#include <conclave-server/user_session.h>
#include <conclave-server/user_sessions.h>

static int roomCreateConnection(ClvRoom* self, const struct ClvUserSession* ownerOfConnection, MonotonicTimeMs now,
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
            clvRoomConnectionInit(roomConnection, self, ownerOfConnection, now);
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

void clvRoomInit(ClvRoom* self, const ClvRoomConfig* config)
{
    self->applicationId = config->applicationId;
    self->applicationVersion = config->applicationVersion;
    self->log = config->log;
    tc_strcpy(self->name, 64, config->roomName);
    self->id = config->indexInRooms;
    self->ownedByConnection = 0;
    self->createdByUserSession = config->createdByUserSession;
    clvRoomConnectionsInit(&self->roomConnections, config->maxMemberCount, config->log);
}

int clvRoomCreateRoomConnection(ClvRoom* self, const struct ClvUserSession* foundUserSession, MonotonicTimeMs now,
                                ClvRoomConnection** outConnection)
{
    if (foundUserSession == 0) {
        CLOG_C_ERROR(&self->log, "creator is null")
    }

    int errorCode = clvRoomConnectionsFindConnection(&self->roomConnections, foundUserSession, outConnection);
    if (errorCode < 0) {
        *outConnection = 0;
        return errorCode;
    }

    if (errorCode == 1) {
        return 0;
    }

    errorCode = roomCreateConnection(self, foundUserSession, now, outConnection);
    if (errorCode < 0) {
        *outConnection = 0;
        return errorCode;
    }

    return 0;
}

void clvRoomCheckValidOwner(ClvUserSessions* sessions, ClvRoom* self)
{
    if (self->ownedByConnection == 0) {
        if (self->roomConnections.connectionCount == 0) {
            return;
        }
        self->ownedByConnection = &self->roomConnections.connections[0];
        return;
    }

    bool shouldDisconnect = clvRoomConnectionShouldDisconnect(self->ownedByConnection);
    if (!shouldDisconnect) {
        return;
    }

    // Disconnect owner
    clvUserSessionsDestroySession(sessions, self->ownedByConnection->owner->userSessionId);
    clvRoomConnectionsDestroyConnection(&self->roomConnections, self->ownedByConnection->id);
    self->ownedByConnection = clvRoomConnectionsFindConnectionWithMostKnowledge(&self->roomConnections);
}

#if defined CLOG_LOG_ENABLED
void clvRoomDebugOutput(const ClvRoom* self)
{
    CLOG_C_INFO(&self->log, "'%s' (members %u, connections %zu)", self->name, 0u, self->roomConnections.connectionCount)
}

#endif
