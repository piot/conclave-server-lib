/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_USERS_H
#define CONCLAVE_SERVER_USERS_H

#include <stdlib.h>

#include <conclave-server/address.h>

struct FldInStream;
struct ClvUser;

typedef struct ClvUsers {
    struct ClvUser* users;
    size_t capacity;
    size_t count;
} ClvUsers;

void clvUsersInit(ClvUsers* self);
void clvUsersReset(ClvUsers* self);
void clvUsersDestroy(ClvUsers* self);
int clvUsersCreate(ClvUsers* sessions, struct ClvUser* user, const ClvAddress* address, struct ClvUser** outUser);
int clvUsersReadAndFind(const ClvUsers* self, const ClvAddress* address, struct FldInStream* stream,
                        struct ClvUser** outUser);
int clvUsersReadLogin(const ClvUsers* self, const ClvAddress* address, struct FldInStream* stream,
                      struct ClvUser** outUser);

#endif
