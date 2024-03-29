/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_REQ_ROOM_CREATE_H
#define CONCLAVE_SERVER_REQ_ROOM_CREATE_H

struct ClvRooms;
struct ClvUserSession;
struct FldOutStream;
struct FldInStream;

#include <monotonic-time/monotonic_time.h>

int clvReqRoomCreate(struct ClvRooms* self, struct ClvUserSession* userSession, MonotonicTimeMs now,
                     struct FldInStream* inStream, struct FldOutStream* outStream);

#endif
