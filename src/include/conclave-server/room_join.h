/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_JOIN_H
#define CONCLAVE_SERVER_ROOM_JOIN_H

struct ClvRoom;
struct FldInStream;
struct ClvRoomConnection;
struct ClvUserSession;

int clvReadAndJoinMembers(struct ClvRoom* session, struct ClvUserSession* ownerOfConnection,
                               struct FldInStream* inStream, struct ClvRoomConnection** createdConnection);

#endif
