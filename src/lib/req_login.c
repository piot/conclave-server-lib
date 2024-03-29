/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-serialize/serialize.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_user_login.h>
#include <conclave-server/server.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <guise-sessions-client/user_session.h>
#include <inttypes.h>

int clvReqUserLogin(ClvServer* self, const struct GuiseSclUserSession* userSession, FldInStream* inStream,
                    FldOutStream* outStream)
{
    ClvSerializeClientNonce clientNonce;
    clvSerializeReadClientNonce(inStream, &clientNonce);

    ClvUserSession* foundSession;
    int err = clvUserSessionsCreate(&self->userSessions, userSession, &foundSession);
    if (err < 0) {
        return err;
    }

    CLOG_C_DEBUG(&self->log, "logged in user '%s' and created user session %" PRIx64,
                 foundSession->guiseUserSession->userName.utf8, foundSession->userSessionId)

    return clvSerializeServerOutLogin(outStream, clientNonce, foundSession->userSessionId);
}
