/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_SERVER_H
#define CONCLAVE_SERVER_SERVER_H

#include <conclave-server/address.h>
#include <conclave-server/room_connection.h>
#include <conclave-server/rooms.h>
#include <conclave-server/transport.h>
#include <conclave-server/user_sessions.h>
#include <conclave-server/users.h>
#include <stdarg.h>
#include <clog/clog.h>

typedef struct ClvServer {
    ClvRooms rooms;
    ClvUsers users;
    ClvUserSessions userSessions;
    Clog log
} ClvServer;

typedef struct ClvResponse {
    ClvServerSendDatagram sendDatagram;
} ClvResponse;

int clvServerInit(ClvServer* self, struct ImprintAllocator* memory, Clog log);
void clvServerDestroy(ClvServer* self);
void clvServerReset(ClvServer* self);
int clvServerFeed(ClvServer* self, const ClvAddress* address, const uint8_t* data, size_t len, ClvResponse* response);

#endif
