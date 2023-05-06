/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_room_create.h>
#include <conclave-server/room.h>
#include <conclave-server/room_connection.h>
#include <conclave-server/room_join.h>
#include <conclave-server/room_member.h>
#include <conclave-server/rooms.h>
#include <conclave-server/serialize.h>
#include <conclave-server/server.h>
#include <conclave-server/user_session.h>
#include <conclave-server/user_sessions.h>
#include <conclave-serialize/types.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <tiny-libc/tiny_libc.h>

int clvReqRoomCreate(ClvRooms* self, const ClvUserSessions* userSessions, const ClvAddress* address,
                     const uint8_t* data, size_t len, FldOutStream* outStream)
{
    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);
    ClvUserSession* foundUserSession;
    int errorCode = clvUserSessionsReadAndFind(userSessions, address, &inStream, &foundUserSession);
    if (errorCode < 0) {
        return errorCode;
    }
    char name[64];
    uint8_t numberOfPlayers;
    uint8_t flags;
    clvReadString(&inStream, name, 64);
    fldInStreamReadUInt8(&inStream, &numberOfPlayers);
    fldInStreamReadUInt8(&inStream, &flags);

    ClvRoom* createdRoom;
    int roomId = clvRoomsCreate(self, name, foundUserSession->user, numberOfPlayers, &createdRoom);
    if (roomId < 0) {
        return roomId;
    }

    CLOG_INFO("room create handle %d '%s' %d", roomId, name, numberOfPlayers)
    ClvRoomConnection* createdConnection;
    errorCode = clvReadAndJoinMembers(createdRoom, foundUserSession, &inStream, &createdConnection);
    if (errorCode < 0) {
        CLOG_WARN("couldn't find game session")
        return errorCode;
    }

    ClvSerializeMember members[8];
    for (size_t i = 0; i < createdConnection->memberCount; ++i) {
        const ClvRoomMember* sourceRoom = createdConnection->members[i];
        members[i].id = sourceRoom->id;
        members[i].localIndex = sourceRoom->localIndex;
        CLOG_INFO("joining participant id:%zu localIndex:%zu", sourceRoom->id, sourceRoom->localIndex)
    }

    return clvSerializeServerOutRoomCreate(outStream, roomId, createdConnection->id, members,
                                           createdConnection->memberCount);
}
