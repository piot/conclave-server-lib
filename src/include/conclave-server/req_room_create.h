/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_REQ_ROOM_CREATE_H
#define CONCLAVE_SERVER_REQ_ROOM_CREATE_H

struct ClvRooms;
struct ClvUserSession;
struct FldOutStream;
struct FldInStream;

int clvReqRoomCreate(struct ClvRooms* self, const struct ClvUserSession* userSession, struct FldInStream* inStream,
                     struct FldOutStream* outStream);

#endif
