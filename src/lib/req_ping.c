/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <conclave-serialize/commands.h>
#include <conclave-serialize/server_in.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_ping.h>
#include <conclave-server/room.h>
#include <conclave-server/room_connection.h>
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
    pingResponse.roomInfo.memberCount = foundRoom->roomConnections.connectionCount;

    GuiseSerializeUserId roomOwnedByUserId = foundRoom->ownedByConnection->owner->guiseUserSession->userId;
    size_t indexOfOwner = 0;
    bool foundOwner = false;

    for (size_t i = 0; i < pingResponse.roomInfo.memberCount; ++i) {
        pingResponse.roomInfo.members[i] = foundRoom->roomConnections.connections[i].owner->userId;
        if (roomOwnedByUserId == pingResponse.roomInfo.members[i]) {
            foundOwner = true;
            indexOfOwner = i;
        }
    }

    if (!foundOwner) {
        CLOG_C_ERROR(&foundRoom->log, "no owner of the room. panic")
    }

    pingResponse.roomInfo.indexOfOwner = (uint8_t) indexOfOwner;

    clvSerializeServerOutPing(outStream, &pingResponse);
}

int clvReqPing(ClvServer* self, const struct ClvUserSession* userSession, MonotonicTimeMs now,
               struct FldInStream* inStream, FldOutStream* outStream)
{
    uint64_t knowledge;

    clvSerializeServerInPing(inStream, &knowledge);

    ClvRoomConnection* foundRoomConnection = userSession->primaryRoomConnection;
    if (foundRoomConnection == nullptr) {
        CLOG_C_NOTICE(&self->log, "user has no room yet")
        return 0;
    }

    clvRoomConnectionOnPing(foundRoomConnection, knowledge, now);

    clvRoomCheckValidOwner(foundRoomConnection->ownedByRoom);

    ClvRoom* foundRoom = foundRoomConnection->ownedByRoom;
    writePingResponse(foundRoom, outStream);

    return 0;
}
