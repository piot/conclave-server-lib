/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <conclave-server/address.h>
#include <conclave-server/user_session.h>
#include <conclave-server/user_sessions.h>

void clvUserSessionsInit(ClvUserSessions* self)
{
    self->userSessionCapacity = 1024;
    self->userSessions = tc_malloc_type_count(ClvUserSession, self->userSessionCapacity);
    tc_mem_clear_type_n(self->userSessions, self->userSessionCapacity);
}

void clvUserSessionsReset(ClvUserSessions* self)
{
    for (size_t i = 0; i < self->userSessionCapacity; ++i) {
        ClvUserSession* session = &self->userSessions[i];
        session->user = 0;
    }
}

void clvUserSessionsDestroy(ClvUserSessions* self)
{
  self->userSessionCapacity = 0;
  tc_free(self->userSessions);
}

int clvUserSessionsCreate(ClvUserSessions* sessions, struct ClvUser* user, const ClvAddress* address,
                          ClvUserSession** outSession)
{
    for (size_t i = 0; i < sessions->userSessionCapacity; ++i) {
        ClvUserSession* session = &sessions->userSessions[i];
        if (session->user == 0) {
            session->address = *address;
            session->user = user;
            *outSession = session;
            return i;
        }
    }
    *outSession = 0;
    return -1;
}

static int userSessionsFind(const ClvUserSessions* self, uint32_t id, const ClvAddress* addr,
                            ClvUserSession** outSession)
{
    if (id >= self->userSessionCapacity) {
        return -2;
    }

    ClvUserSession* foundSession = &self->userSessions[id];
    if (!clvAddressEqual(addr, &foundSession->address)) {
        char addrTemp[64];
        CLOG_SOFT_ERROR("wrong address %s vs %s", clvAddressToString(addr, addrTemp, 64),
                        clvAddressToString(&foundSession->address, addrTemp, 64));
        *outSession = 0;
        return -3;
    }

    *outSession = foundSession;

    return 0;
}

int clvUserSessionsReadAndFind(const ClvUserSessions* self, const ClvAddress* address, FldInStream* stream,
                               ClvUserSession** outSession)
{
    uint32_t sessionId;
    fldInStreamReadUInt32(stream, &sessionId);

    int errorCode = userSessionsFind(self, sessionId, address, outSession);
    if (errorCode < 0) {
        CLOG_WARN("couldn't find user session %d", sessionId);
        return errorCode;
    }

    CLOG_DEBUG("found user session %d", sessionId);

    return 0;
}
