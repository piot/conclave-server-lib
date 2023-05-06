/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOMS_H
#define CONCLAVE_SERVER_ROOMS_H

#include <stdlib.h>

struct ClvRoom;
struct FldInStream;
struct ClvUser;
struct ClvRoomConnection;
struct ImprintAllocator;

typedef struct ClvRooms {
    struct ClvRoom* rooms;
    size_t capacity;
    size_t count;
    struct ImprintAllocator* pageAllocator;
} ClvRooms;

void clvRoomsInit(ClvRooms* self, struct ImprintAllocator* allocator);
void clvRoomsReset(ClvRooms* self);
void clvRoomsDestroy(ClvRooms* self);
int clvRoomsCreate(ClvRooms* self, const char* name, struct ClvUser* user, size_t maxRoomCount,
                   struct ClvRoom** outRoom);
int clvRoomsReadAndFind(ClvRooms* self, struct FldInStream* stream, struct ClvRoom** outRoom);
int clvRoomsReadNameAndFind(ClvRooms* self, struct FldInStream* stream, struct ClvRoom** outRoom);
int clvRoomsReadAndFindRoomConnection(ClvRooms* self, struct FldInStream* stream, const struct ClvUser* requiredUser,
                                      struct ClvRoomConnection** outRoomConnection);
#endif
