/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_REQ_ROOM_JOIN_H
#define CONCLAVE_SERVER_REQ_ROOM_JOIN_H

#include <conclave-server/address.h>

#include <stdint.h>

struct ClvServer;
struct FldOutStream;
struct FldInStream;
struct ClvUserSession;

int clvReqRoomJoin(struct ClvServer* self, const struct ClvUserSession* foundUserSession, struct FldInStream* inStream,
                   struct FldOutStream* outStream);

#endif
