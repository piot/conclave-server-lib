/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_REQ_ROOM_CREATE_H
#define CONCLAVE_SERVER_REQ_ROOM_CREATE_H

#include <conclave-server/address.h>

struct ClvRooms;
struct ClvUserSessions;
struct FldOutStream ;

int clvReqRoomCreate(struct ClvRooms* self, const struct ClvUserSessions* userSessions, const ClvAddress* address,
                     const uint8_t* data, size_t len, struct FldOutStream* outStream);

#endif
