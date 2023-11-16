/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_REQ_PING_H
#define CONCLAVE_SERVER_REQ_PING_H

#include <stdint.h>

struct ClvServer;
struct FldOutStream;
struct FldInStream;
struct ClvUserSession;

int clvReqPing(struct ClvServer* self, const struct ClvUserSession* foundUserSession, struct FldInStream* inStream,
                   struct FldOutStream* outStream);

#endif
