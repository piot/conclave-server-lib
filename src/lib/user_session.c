/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <conclave-server/user_session.h>
#include <guise-sessions-client/user_session.h>
#include <datagram-reassembly/write.h>

void clvUserSessionInit(ClvUserSession* self, ClvSerializeUserSessionId id,
                        const struct GuiseSclUserSession* guiseUserSession, Clog log)
{
    self->log = log;
    self->userSessionId = id;
    self->userId = guiseUserSession->userId;
    self->guiseUserSession = guiseUserSession;
    self->primaryRoomConnection = 0;
    datagramReassemblyWriteInit(&self->write);
}
