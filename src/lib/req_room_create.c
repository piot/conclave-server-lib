/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
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
    char name[64];
    uint8_t numberOfPlayers;
    uint8_t flags;
    clvSerializeReadString(inStream, name, 64);
    fldInStreamReadUInt8(inStream, &numberOfPlayers);
    fldInStreamReadUInt8(inStream, &flags);

    if (foundUserSession->primaryRoomConnection != 0) {
        CLOG_C_NOTICE(&self->log, "user session already have created a room")
        return 0;
    }

    ClvRoom* createdRoom;
    int worked = clvRoomsCreate(self, foundUserSession->guiseUserSession, name, numberOfPlayers, &createdRoom);
    if (worked < 0) {
        return worked;
    }

    CLOG_C_INFO(&self->log, "room create handle %zu '%s' %d", createdRoom->id, name, numberOfPlayers)
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
