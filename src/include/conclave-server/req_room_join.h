/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_REQ_ROOM_JOIN_H
#define CONCLAVE_SERVER_REQ_ROOM_JOIN_H

#include <monotonic-time/monotonic_time.h>
#include <stdint.h>

struct ClvServer;
struct FldOutStream;
struct FldInStream;
struct ClvUserSession;

int clvReqRoomJoin(struct ClvServer* self, struct ClvUserSession* foundUserSession, MonotonicTimeMs now,
                   struct FldInStream* inStream, struct FldOutStream* outStream);

#endif
