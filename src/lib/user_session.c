/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <conclave-server/user_session.h>

void clvUserSessionInit(ClvUserSession* self, ClvSerializeUserSessionId id, const struct GuiseSclUserSession* guiseUserSession,
                        Clog log)
{
    self->log = log;
    self->userSessionId = id;
    self->guiseUserSession = guiseUserSession;
}
