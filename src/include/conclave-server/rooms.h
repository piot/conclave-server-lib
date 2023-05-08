/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOMS_H
#define CONCLAVE_SERVER_ROOMS_H

#include <stdlib.h>
#include <clog/clog.h>

struct ClvRoom;
struct FldInStream;
struct ClvUserSession;
struct ClvRoomConnection;
struct ImprintAllocator;
struct ClvUser;

typedef struct ClvRooms {
    struct ClvRoom* rooms;
    size_t capacity;
    size_t count;
    struct ImprintAllocator* pageAllocator;
    char prefix[32];
    Clog log;
} ClvRooms;

void clvRoomsInit(ClvRooms* self, struct ImprintAllocator* allocator, Clog log);
void clvRoomsReset(ClvRooms* self);
void clvRoomsDestroy(ClvRooms* self);
int clvRoomsCreate(ClvRooms* self, const char* name, const struct ClvUser* user, size_t maxRoomCount,
                   struct ClvRoom** outRoom);
int clvRoomsReadAndFind(ClvRooms* self, struct FldInStream* stream, struct ClvRoom** outRoom);

int clvRoomsReadAndFindRoomConnection(ClvRooms* self, struct FldInStream* stream, const struct ClvUserSession* requiredUserSession,
                                      struct ClvRoomConnection** outRoomConnection);
#endif
