/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "cipher.h"
#include <clog/clog.h>
#include <conclave-serialize/server_in.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_challenge.h>
#include <conclave-server/server.h>
#include <conclave-server/user.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>

int clvReqChallenge(ClvServer* self, const ClvAddress* address, const uint8_t* data, size_t len,
                    struct FldOutStream* outStream)
{
    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    ClvSerializeClientNonce clientNonce;
    clvSerializeServerInChallenge(&inStream, &clientNonce);

    // Challenge is done to avoid at least the simplest forms of IP spoofing
    uint64_t constructedChallenge = extremelyUnsecureCipher(clientNonce, self->secretChallengeKey);

    CLOG_C_VERBOSE(&self->log, "received challenge request from client nonce %016lX and constructed challenge %016lX",
                   clientNonce, constructedChallenge)

    return clvSerializeServerOutChallenge(outStream, clientNonce, constructedChallenge);
}
