/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_REQ_PACKET_H
#define CONCLAVE_SERVER_REQ_PACKET_H

#include <stddef.h>
#include <stdint.h>

struct ClvServer;
struct FldOutStream;
struct FldInStream;
struct ClvUserSession;
struct ClvResponse;

int clvReqPacket(struct ClvServer* self, const struct ClvUserSession* userSession, struct FldInStream* inStream,
                 struct ClvResponse* response);

#endif
