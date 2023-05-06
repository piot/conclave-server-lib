/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_packet.h>
#include <conclave-server/req_user_login.h>
#include <conclave-server/server.h>
#include <conclave-server/user.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>

int clvReqPacket(ClvServer* self, const uint8_t* data, size_t len, FldOutStream* outStream)
{
    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    ClvRoomConnection* foundRoomConnection;

    int errorCode = clvRoomsReadAndFindRoomConnection(&self->rooms, &inStream, &foundRoomConnection);

    // fldReadUInt8(targetConnectionId);

    return 0;
}
