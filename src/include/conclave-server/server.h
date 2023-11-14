/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_SERVER_H
#define CONCLAVE_SERVER_SERVER_H

#include <clog/clog.h>
#include <conclave-server/room_connection.h>
#include <conclave-server/rooms.h>
#include <conclave-server/user_sessions.h>
#include <guise-sessions-client/address.h>
#include <guise-sessions-client/client.h>
#include <stdarg.h>

typedef struct ClvServer {
    ClvRooms rooms;
    ClvUserSessions userSessions;
    Clog log;
    GuiseSclClient guiseSclClient;
} ClvServer;

typedef struct ClvResponse {
    struct DatagramTransportOut* transportOut;
} ClvResponse;

int clvServerInit(ClvServer* self, DatagramTransport transportToGuiseServer,
                  GuiseSerializeUserSessionId assignedSessionIdForThisRelayServer, struct ImprintAllocator* memory,
                  Clog log);
void clvServerDestroy(ClvServer* self);
void clvServerReset(ClvServer* self);
int clvServerFeed(ClvServer* self, const GuiseSclAddress* address, const uint8_t* data, size_t len,
                  ClvResponse* response);
int clvServerUpdate(ClvServer* self);

#endif
