/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_REQ_CHALLENGE_H
#define CONCLAVE_SERVER_REQ_CHALLENGE_H

#include <conclave-server/address.h>

struct ClvServer;
struct FldOutStream;

int clvReqChallenge(struct ClvServer* self, const ClvAddress* address, const uint8_t* data, size_t len,
                    struct FldOutStream* outStream);

#endif
