/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_REQ_USER_LOGIN_H
#define CONCLAVE_SERVER_REQ_USER_LOGIN_H

struct ClvServer;
struct FldOutStream;
struct FldInStream;
struct GuiseSclUserSession;

int clvReqUserLogin(struct ClvServer* self, const struct GuiseSclUserSession* userSession, struct FldInStream* inStream,
                    struct FldOutStream* outStream);

#endif
