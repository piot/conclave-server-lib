/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
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

    ClvUser* foundUser;
    int errorCode = clvUsersReadLogin(&self->users, address, &inStream, &foundUser);
    if (errorCode < 0) {
        return errorCode;
    }

    ClvUserSession* foundSession;
    int sessionId = clvUserSessionsCreate(&self->userSessions, foundUser, address, &foundSession);
    if (sessionId < 0) {
        return sessionId;
    }

    CLOG_C_DEBUG(&self->log, "logged in user '%s' and created user session %d", foundUser->name, sessionId);

    clvSerializeServerOutLogin(outStream, sessionId);
    return 0;
}
