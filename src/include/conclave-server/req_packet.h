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

int clvReqPacket(struct ClvServer* self, const uint8_t* data, size_t len, struct FldOutStream* response);

#endif
