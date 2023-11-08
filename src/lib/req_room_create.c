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

int clvReqRoomCreate(ClvRooms* self, const ClvUserSession* foundUserSession, FldInStream* inStream,
                     FldOutStream* outStream)
{
    char name[64];
    uint8_t numberOfPlayers;
    uint8_t flags;
    clvSerializeReadString(inStream, name, 64);
    fldInStreamReadUInt8(inStream, &numberOfPlayers);
    fldInStreamReadUInt8(inStream, &flags);

    ClvRoom* createdRoom;
    int worked = clvRoomsCreate(self, name, foundUserSession, numberOfPlayers, &createdRoom);
    if (worked < 0) {
        return worked;
    }

    CLOG_C_INFO(&self->log, "room create handle %d '%s' %d", roomId, name, numberOfPlayers)
    ClvRoomConnection* createdConnection;

    int errorCode = clvRoomCreateRoomConnection(createdRoom, foundUserSession, &createdConnection);
    if (errorCode < 0) {
        CLOG_C_WARN(&self->log, "couldn't creat room connection")
        return errorCode;
    }

    return clvSerializeServerOutRoomCreate(outStream, (ClvSerializeRoomId) createdRoom->id, createdConnection->id);
}
