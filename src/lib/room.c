/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-server/room.h>
#include <conclave-server/room_connection.h>
#include <conclave-server/room_connections.h>

/// Creates a room connection for the specified room.
/// @param self
/// @param ownerOfConnection
/// @param joinInfo
/// @param localRoomCount
/// @param outConnection
/// @return error code
static int roomCreateConnection(ClvRoom* self, const struct ClvUserSession* ownerOfConnection,
                                const ClvMemberJoinInfo* joinInfo, size_t localRoomCount,
                                ClvRoomConnection** outConnection)
{
    ClvRoomConnections* connections = &self->roomConnections;

    for (size_t i = 0; i < connections->capacityCount; ++i) {
        ClvRoomConnection* roomConnection = &connections->connections[i];
        if (roomConnection->owner == 0) {
            struct ClvRoomMember* createdMembers[16];
            /*
            int errorCode = clvRoomMembersJoin(&self->members, joinInfo, localRoomCount, createdMembers);
            if (errorCode < 0) {
                *outConnection = 0;
                return errorCode;
            }
            */
            roomConnection->owner = ownerOfConnection;
            if (ownerOfConnection == 0) {
                CLOG_ERROR("owner is null");
            }
            roomConnection->id = i;
            clvRoomConnectionInit(roomConnection, self->allocator, 2 * 1024, createdMembers, localRoomCount);
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
    //clvRoomMembersDestroy(&self->members);
    clvRoomConnectionsDestroy(&self->roomConnections);
}

int clvRoomCreateRoomConnection(ClvRoom* self, const struct ClvUserSession* foundUserSession,
   const ClvMemberJoinInfo* joinInfo, size_t localMemberCount,
                                ClvRoomConnection** outConnection)
{
    if (foundUserSession == 0) {
        CLOG_ERROR("creator is null");
        return 0;
    }

    int errorCode = clvRoomConnectionsFindConnection(&self->roomConnections, foundUserSession, outConnection);
    if (errorCode < 0) {
        *outConnection = 0;
        return errorCode;
    }

    if (errorCode == 1) {
        return 0;
    }

    errorCode = roomCreateConnection(self, foundUserSession, joinInfo, localMemberCount, outConnection);
    if (errorCode < 0) {
        *outConnection = 0;
        return errorCode;
    }

    return 0;
}

void clvRoomDebugOutput(const ClvRoom* self)
{
    CLOG_INFO("room: %16lX '%s' (members %zu, connections %zu)", self->id, self->name, 0,
              self->roomConnections.connectionCount);
}
