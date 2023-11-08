/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <conclave-serialize/commands.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_room_create.h>
#include <conclave-server/req_room_rejoin.h>
#include <conclave-server/room.h>
#include <conclave-server/room_connection.h>
#include <conclave-server/rooms.h>
#include <conclave-server/server.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>

int clvReqRoomReJoin(ClvServer* self, const struct ClvUserSession* userSession, struct FldInStream* inStream,
                     FldOutStream* outStream)
{
    ClvRoomConnection* foundRoomConnection;

    int errorCode = clvRoomsReadAndFindRoomConnection(&self->rooms, inStream, userSession, &foundRoomConnection);
    if (errorCode < 0) {
        return errorCode;
    }

    clvSerializeWriteCommand(outStream, clvSerializeCmdRoomReJoinResponse, "ServerOut");

    // TODO: send connection and member Ids

    return 0;
}
