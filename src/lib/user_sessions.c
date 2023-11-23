/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-serialize/serialize.h>
#include <conclave-server/unique_id.h>
#include <conclave-server/user_session.h>
#include <conclave-server/user_sessions.h>
#include <flood/in_stream.h>
#include <guise-sessions-client/user_session.h>
#include <inttypes.h>

void clvUserSessionsInit(ClvUserSessions* self, Clog log)
{
    self->log = log;
    self->userSessionCapacity = 1024;
    self->userSessions = tc_malloc_type_count(ClvUserSession, self->userSessionCapacity);
    self->userSessionCount = 0;
    tc_mem_clear_type_n(self->userSessions, self->userSessionCapacity);
}

void clvUserSessionsReset(ClvUserSessions* self)
{
    for (size_t i = 0; i < self->userSessionCapacity; ++i) {
        ClvUserSession* session = &self->userSessions[i];
        session->guiseUserSession = 0;
    }
}

void clvUserSessionsDestroy(ClvUserSessions* self)
{
    self->userSessionCapacity = 0;
    tc_free(self->userSessions);
}

int clvUserSessionsCreate(ClvUserSessions* sessions, const GuiseSclUserSession* guiseUserSession,
                          ClvUserSession** outSession)
{
    for (size_t i = 0; i < sessions->userSessionCapacity; ++i) {
        ClvUserSession* session = &sessions->userSessions[i];
        if (session->guiseUserSession == 0) {
            Clog userSessionLog;
            userSessionLog.config = sessions->log.config;
            tc_snprintf(session->prefix, 32, "%s/%zu", sessions->log.constantPrefix, i);
            userSessionLog.constantPrefix = session->prefix;
            ClvSerializeUserSessionId uniqueSessionId = clvGenerateUniqueIdFromIndex(i);
            clvUserSessionInit(session, uniqueSessionId, guiseUserSession, userSessionLog);
            sessions->userSessionCount++;
            *outSession = session;
            return 0;
        }
    }
    *outSession = 0;
    return -1;
}

void clvUserSessionsDestroySession(ClvUserSessions* self, ClvSerializeUserSessionId sessionId)
{
    CLOG_C_DEBUG(&self->log, "destroy user session %" PRIx64, sessionId)
    size_t index = clvUniqueIdGetIndex(sessionId);
    if (index >= self->userSessionCapacity) {
        CLOG_C_ERROR(&self->log, "illegal session id")
    }

    CLOG_C_DEBUG(&self->log, "destroy user session at index %zu", index)

    ClvUserSession* userSession = &self->userSessions[index];
    CLOG_ASSERT(userSession->guiseUserSession != 0, "trying to allocate an empty user session")
    userSession->guiseUserSession = 0;
    userSession->userSessionId = 0;
    userSession->userId = 0;
    self->userSessionCount--;
}

static int userSessionsFind(const ClvUserSessions* self, ClvSerializeUserSessionId uniqueId,
                            const GuiseSclAddress* address, ClvUserSession** outSession)
{
    size_t index = clvUniqueIdGetIndex(uniqueId);
    if (index >= self->userSessionCapacity) {
        return -2;
    }

    ClvUserSession* foundSession = &self->userSessions[index];

    if (foundSession->userSessionId == 0) {
        CLOG_C_NOTICE(&self->log, "user session id is not valid: %" PRIX64, uniqueId)
        return -1;
    }

    if (foundSession->userSessionId != uniqueId) {
        CLOG_C_SOFT_ERROR(&self->log, "wrong user session id, got %" PRIX64 " but wanted %" PRIX64, uniqueId,
                          foundSession->userSessionId)
    }
    (void) address;
    /*
    if (!clvAddressEqual(addr, &foundSession->address)) {
        char addrTemp[64];
        CLOG_C_SOFT_ERROR(&self->log, "wrong address %s vs %s", clvAddressToString(addr, addrTemp, 64),
                          clvAddressToString(&foundSession->address, addrTemp, 64));
        *outSession = 0;
        return -3;
    }
*/
    *outSession = foundSession;

    return 0;
}

int clvUserSessionsReadAndFind(const ClvUserSessions* self, const GuiseSclAddress* address, FldInStream* stream,
                               ClvUserSession** outSession)
{

    ClvSerializeUserSessionId userSessionId;
    clvSerializeReadUserSessionId(stream, &userSessionId);

    int errorCode = userSessionsFind(self, userSessionId, address, outSession);
    if (errorCode < 0) {
        CLOG_C_WARN(&self->log, "couldn't find user session %" PRIX64, userSessionId)
        return errorCode;
    }

    return 0;
}
