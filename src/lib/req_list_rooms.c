/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-room/room.h>
#include <conclave-serialize/server_in.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_list_rooms.h>
#include <conclave-server/server.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <guise-sessions-client/user_session.h>

int clvReqListRooms(ClvServer* self, const ClvUserSession* foundUserSession, FldInStream* inStream,
                    FldOutStream* outStream)
{
    (void) foundUserSession;

    ClvSerializeListRoomsOptions listRoomsOptions;

    clvSerializeServerInListRooms(inStream, &listRoomsOptions);

    CLOG_C_DEBUG(&self->log, "query list of rooms, max:%d", listRoomsOptions.maximumCount)

    ClvSerializeListRoomsResponseOptions response;
    size_t availableRoomCount = self->rooms.count;
    size_t roomCountToSend = listRoomsOptions.maximumCount > availableRoomCount ? availableRoomCount
                                                                                : listRoomsOptions.maximumCount;
    roomCountToSend = roomCountToSend > 16 ? 16 : roomCountToSend;

    size_t roomCountFilledIn = 0;
    for (size_t i = 0; i < self->rooms.capacity; ++i) {
        const ClvRoom* room = &self->rooms.rooms[i];
        if (room->createdByUserSession == 0) {
            continue;
        }
        ClvSerializeRoomInfo* info = &response.roomInfos[roomCountFilledIn];
        info->roomId = (ClvSerializeRoomId) room->id;
        info->applicationId = room->applicationId;
        info->applicationVersion = room->applicationVersion;
        info->ownerUserId = room->ownedByConnection ? room->ownedByConnection->owner->userId : 0;
        info->roomName = room->name;
        info->externalStateOctetCount = 0;
        info->externalStateOctets = 0;
        roomCountFilledIn++;
        if (roomCountFilledIn >= roomCountToSend) {
            break;
        }
    }
    response.roomInfoCount = roomCountFilledIn;

    CLOG_C_DEBUG(&self->log, "send list of rooms, found:%zu", response.roomInfoCount)

    clvSerializeServerOutListRooms(outStream, &response);

    return 0;
}
