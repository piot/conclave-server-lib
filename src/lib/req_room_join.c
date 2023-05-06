/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_room_join.h>
#include <conclave-server/room.h>
#include <conclave-server/server.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>

int clvReqRoomJoin(ClvServer* self, const ClvAddress* address, const uint8_t* data, size_t len, FldOutStream* outStream)
{
    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    ClvUserSession* foundUserSession;
    int errorCode = clvUserSessionsReadAndFind(&self->userSessions, address, &inStream, &foundUserSession);
    if (errorCode < 0) {
        CLOG_WARN("couldn't find user session");
        return errorCode;
    }
    ClvRoom* foundRoom;
    errorCode = clvRoomsReadNameAndFind(&self->rooms, &inStream, &foundRoom);
    if (errorCode < 0) {
        CLOG_WARN("couldn't find room");
        return errorCode;
    }
    ClvRoomConnection* createdConnection;

    errorCode = clvRoomCreateRoomConnection(foundRoom, foundUserSession, &createdConnection);
    if (errorCode < 0) {
        CLOG_WARN("couldn't join room")
        return errorCode;
    }

    CLOG_DEBUG("joined room %lu", foundRoom->id);
    clvSerializeServerOutRoomJoin(outStream, foundRoom->id, createdConnection->id);

    return 0;
}