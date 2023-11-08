/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_USER_SESSION_H
#define CONCLAVE_SERVER_USER_SESSION_H

#include <clog/clog.h>
#include <conclave-serialize/types.h>

struct GuiseSclUserSession;

typedef struct ClvUserSession {
    ClvSerializeUserSessionId userSessionId;
    const struct GuiseSclUserSession* guiseUserSession;
    Clog log;
    char prefix[32];
} ClvUserSession;

void clvUserSessionInit(ClvUserSession* self, ClvSerializeUserSessionId userSessionId,
                        const struct GuiseSclUserSession* guiseUserSession, Clog log);

#endif
