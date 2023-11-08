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

int clvReqRoomJoin(ClvServer* self, const ClvUserSession* foundUserSession, FldInStream* inStream,
                   FldOutStream* outStream)
{
    ClvRoom* foundRoom;
    int errorCode = clvRoomsReadAndFind(&self->rooms, inStream, &foundRoom);
    if (errorCode < 0) {
        CLOG_C_WARN(&self->log, "couldn't find room");
        return errorCode;
    }
    ClvRoomConnection* createdConnection;

    errorCode = clvRoomCreateRoomConnection(foundRoom, foundUserSession, &createdConnection);
    if (errorCode < 0) {
        CLOG_C_WARN(&self->log, "couldn't join room")
        return errorCode;
    }

    CLOG_C_DEBUG(&self->log, "joined room %lu", foundRoom->id);
    clvSerializeServerOutRoomJoin(outStream, (ClvSerializeRoomId) foundRoom->id, createdConnection->id);

    return 0;
}
