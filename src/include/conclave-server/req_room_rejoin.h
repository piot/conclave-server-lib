/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_REQ_ROOM_REJOIN_H
#define CONCLAVE_SERVER_REQ_ROOM_REJOIN_H

#include <stdint.h>

struct ClvServer;
struct FldOutStream;
struct FldInStream;
struct ClvUserSession;

int clvReqRoomReJoin(struct ClvServer* self, const struct ClvUserSession* userSession, struct FldInStream* inStream,
                     struct FldOutStream* outStream);

#endif
