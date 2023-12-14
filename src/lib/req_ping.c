/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <conclave-room/room.h>
#include <conclave-room/room_connection.h>
#include <conclave-serialize/commands.h>
#include <conclave-serialize/server_in.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_ping.h>
#include <conclave-server/rooms.h>
#include <conclave-server/server.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <guise-sessions-client/user_session.h>

#define nullptr ((void*) 0)

static void writePingResponse(ClvRoom* foundRoom, FldOutStream* outStream)
{
    ClvSerializePingResponseOptions pingResponse;
    pingResponse.roomInfo.memberCount = 0;

    GuiseSerializeUserId roomOwnedByUserId = foundRoom->ownedByConnection->owner->guiseUserSession->userId;
    size_t indexOfOwner = 0;
    bool foundOwner = false;

    for (size_t i = 0; i < foundRoom->roomConnections.capacityCount; ++i) {
        ClvRoomConnection* roomConnection = &foundRoom->roomConnections.connections[i];
        if (roomConnection->owner == 0) {
            continue;
        }
        size_t index = pingResponse.roomInfo.memberCount++;
        pingResponse.roomInfo.members[index] = roomConnection->owner->userId;
        if (roomOwnedByUserId == roomConnection->owner->userId) {
            foundOwner = true;
            indexOfOwner = index;
        }
    }

    if (!foundOwner) {
        CLOG_C_ERROR(&foundRoom->log, "no owner of the room. panic")
    }

    pingResponse.roomInfo.indexOfOwner = (uint8_t) indexOfOwner;
    pingResponse.term = foundRoom->term;
    pingResponse.version = foundRoom->version;

    clvSerializeServerOutPing(outStream, &pingResponse);
}

int clvReqPing(ClvServer* self, const struct ClvUserSession* userSession, MonotonicTimeMs now,
               struct FldInStream* inStream, FldOutStream* outStream)
{
    uint64_t knowledge;

    ClvSerializeTerm term;
    ClvSerializeConnectedToOwnerState hasConnectionToOwner;

    clvSerializeServerInPing(inStream, &knowledge, &term, &hasConnectionToOwner);

    ClvRoomConnection* foundRoomConnection = userSession->primaryRoomConnection;
    if (foundRoomConnection == nullptr) {
        CLOG_C_NOTICE(&self->log, "user has no room yet")
        return 0;
    }

    ClvRoom* room = foundRoomConnection->ownedByRoom;
    clvRoomConnectionsUpdate(&room->roomConnections, now);

    clvRoomConnectionOnPing(foundRoomConnection, knowledge, now);

    if (term == room->term) {
        foundRoomConnection->hasConnectionToOwner = hasConnectionToOwner;
        bool shouldSelectOtherOwner = clvRoomConnectionsHaveMostLostConnectionToOwner(&room->roomConnections);
        if (shouldSelectOtherOwner) {
            clvRoomSelectNewOwner(room, room->ownedByConnection);
        }
    }

    // TODO: &self->userSessions,
    clvRoomCheckForDisconnections(room);

    // If owner connection is disconnected or no owner is assigned
    if (room->ownedByConnection == 0 || room->ownedByConnection->owner == 0) {
        clvRoomSelectNewOwner(room, 0);
    }

    if (foundRoomConnection->owner == 0) {
        // TODO: Send ping response that connection is deleted
        return 0;
    }

    writePingResponse(room, outStream);

    return 0;
}
