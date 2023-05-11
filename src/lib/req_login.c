/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "cipher.h"
#include <clog/clog.h>
#include <conclave-serialize/server_in.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_user_login.h>
#include <conclave-server/server.h>
#include <conclave-server/user.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>

int clvReqUserLogin(ClvServer* self, const ClvAddress* address, const uint8_t* data, size_t len,
                    struct FldOutStream* outStream)
{
    if (len == 0) {
        return -2;
    }

    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    ClvSerializeClientNonce clientNonce;
    ClvSerializeServerChallenge serverChallengeFromClient;
    char username[32];

    clvSerializeServerInLogin(&inStream, &clientNonce, &serverChallengeFromClient, username, 32);

    // Challenge is done to avoid at least the simplest forms of IP spoofing
    uint64_t calculatedClientNonce = extremelyUnsecureCipher(serverChallengeFromClient, self->secretChallengeKey);
    if (calculatedClientNonce != clientNonce) {
        CLOG_C_SOFT_ERROR(&self->log, "challenge didnt work out")
        return -2;
    }

    CLOG_C_DEBUG(&self->log, "challenge was approved from client nonce %016lX", clientNonce);

    ClvUser* foundUser;
    int errorCode = clvUsersReadLogin(&self->users, username, &foundUser);
    if (errorCode < 0) {
        return errorCode;
    }

    ClvUserSession* foundSession;
    int err = clvUserSessionsCreate(&self->userSessions, foundUser, address, &foundSession);
    if (err < 0) {
        return err;
    }

    CLOG_C_DEBUG(&self->log, "logged in user '%s' and created user session %016lX", foundUser->name,
                 foundSession->userSessionId);

    clvSerializeServerOutLogin(outStream, clientNonce, foundSession->userSessionId);
    return 0;
}
