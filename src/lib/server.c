/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-serialize/commands.h>
#include <conclave-serialize/debug.h>
#include <conclave-server/address.h>
#include <conclave-server/req_packet.h>
#include <conclave-server/req_room_create.h>
#include <conclave-server/req_room_join.h>
#include <conclave-server/req_room_rejoin.h>
#include <conclave-server/req_user_login.h>
#include <conclave-server/room.h>
#include <conclave-server/rooms.h>
#include <conclave-server/server.h>
#include <conclave-server/user.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>

int clvServerFeed(ClvServer* self, const ClvAddress* address, const uint8_t* data, size_t len, ClvResponse* response)
{
    CLOG_VERBOSE("clvServerFeed: feed: %s octetCount: %zu", clvSerializeCmdToString(data[0]), len)
#define UDP_MAX_SIZE (1200)
    static uint8_t buf[UDP_MAX_SIZE];
    FldOutStream outStream;
    fldOutStreamInit(&outStream, buf, UDP_MAX_SIZE);
    int result = -1;
    switch (data[0]) {
        case clvSerializeCmdLogin:
            result = clvReqUserLogin(self, address, data + 1, len - 1, &outStream);
            break;
        default: {
            FldInStream inStream;
            fldInStreamInit(&inStream, data + 1, len - 1);
            const ClvUserSession* foundUserSession;
            int err = clvUserSessionsReadAndFind(&self->userSessions, address, &inStream, &foundUserSession);
            if (err < 0) {
                return err;
            }
            switch (data[0]) {
                case clvSerializeCmdRoomCreate:
                    result = clvReqRoomCreate(&self->rooms, foundUserSession, &inStream, &outStream);
                    break;
                case clvSerializeCmdRoomJoin:
                    result = clvReqRoomJoin(self, foundUserSession, &inStream, &outStream);
                    break;
                case clvSerializeCmdRoomReJoin:
                    result = clvReqRoomReJoin(self, foundUserSession, &inStream, &outStream);
                    break;
                case clvSerializeCmdPacket:
                    return clvReqPacket(self, foundUserSession, &inStream, response);
                default:
                    CLOG_SOFT_ERROR("clvServerFeed: unknown command %02X", data[0]);
                    return 0;
            }
            break;
        }
    }

    if (result < 0) {
        return result;
    }

    return response->sendDatagram.send(response->sendDatagram.self, address, buf, outStream.pos);
}

int clvServerInit(ClvServer* self, struct ImprintAllocator* memory)
{
    clvRoomsInit(&self->rooms, memory);
    clvUserSessionsInit(&self->userSessions);
    clvUsersInit(&self->users);

    return 0;
}

void clvServerReset(ClvServer* self)
{
    clvRoomsReset(&self->rooms);
    clvUserSessionsReset(&self->userSessions);
    clvUsersReset(&self->users);
}

void clvServerDestroy(ClvServer* self)
{
    clvRoomsDestroy(&self->rooms);
    clvUserSessionsDestroy(&self->userSessions);
    clvUsersDestroy(&self->users);
}
