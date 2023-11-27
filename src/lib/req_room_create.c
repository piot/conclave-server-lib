/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-serialize/server_in.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_room_create.h>
#include <conclave-server/room.h>
#include <conclave-server/room_connection.h>
#include <conclave-server/rooms.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <guise-sessions-client/client.h>

int clvReqRoomCreate(ClvRooms* self, ClvUserSession* foundUserSession, MonotonicTimeMs now, FldInStream* inStream,
                     FldOutStream* outStream)
{
    ClvSerializeRoomCreateOptions createRoomOptions;

    int deserializeResult = clvSerializeServerInCreateRoom(inStream, &createRoomOptions);
    if (deserializeResult < 0) {
        return deserializeResult;
    }

    if (foundUserSession->primaryRoomConnection != 0) {
        CLOG_C_NOTICE(&self->log, "user session already have created a room")
        return 0;
    }

    ClvRoom* createdRoom;

    ClvRoomCreateData data;
    data.applicationId = createRoomOptions.applicationId;
    data.applicationVersion = createRoomOptions.applicationVersion;
    data.createdByUserSession = foundUserSession->guiseUserSession;
    data.roomName = createRoomOptions.name;
    data.maxMemberCount = createRoomOptions.maxNumberOfPlayers;

    int worked = clvRoomsCreate(self, &data, &createdRoom);
    if (worked < 0) {
        return worked;
    }

    CLOG_C_INFO(&self->log, "room create handle %zu '%s' %d", createdRoom->id, createRoomOptions.name,
                createRoomOptions.maxNumberOfPlayers)

    ClvRoomConnection* createdConnection;
    int errorCode = clvRoomCreateRoomConnection(createdRoom, foundUserSession, now, &createdConnection);
    if (errorCode < 0) {
        CLOG_C_WARN(&self->log, "couldn't creat room connection")
        return errorCode;
    }

    foundUserSession->primaryRoomConnection = createdConnection;
    createdRoom->ownedByConnection = createdConnection;

    return clvSerializeServerOutRoomCreate(outStream, (ClvSerializeRoomId) createdRoom->id, createdConnection->id);
}
