/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-serialize/serialize.h>
#include <conclave-server/room.h>
#include <conclave-server/room_connection.h>
#include <conclave-server/rooms.h>
#include <conclave-server/serialize.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <imprint/allocator.h>

void clvRoomsInit(ClvRooms* self, struct ImprintAllocator* allocator)
{
    self->capacity = 1024;
    self->rooms = IMPRINT_ALLOC_TYPE_COUNT(allocator, ClvRoom, self->capacity);
    tc_mem_clear_type_n(self->rooms, self->capacity);
    for (size_t i = 0; i < self->capacity; ++i) {
        self->rooms[i].id = i;
    }
    self->pageAllocator = allocator;
    self->count = 0;
}

void clvRoomsReset(ClvRooms* self)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        ClvRoom* room = &self->rooms[i];
        if (room->name) {
            tc_free((void*) room->name);
            room->name = 0;
        }
        /// clvRoomsDestroy(&room->members);
        clvRoomConnectionsDestroy(&room->roomConnections);
    }
    self->count = 0;
}

void clvRoomsDestroy(ClvRooms* self)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        ClvRoom* room = &self->rooms[i];
        clvRoomDestroy(room);
    }
}

int clvRoomsCreate(ClvRooms* self, const char* name, struct ClvUser* user, size_t maxRoomCount, ClvRoom** outSession)
{
    for (size_t i = 1; i < self->capacity; ++i) {
        ClvRoom* room = &self->rooms[i];
        if (room->name == 0) {
            room->name = tc_str_dup(name);
            room->id = i;
            room->ownedByUser = user;
            room->allocator = self->pageAllocator;
            // TODO: clvRoomsInit(&room->members, maxRoomCount);
            clvRoomConnectionsInit(&room->roomConnections, maxRoomCount);
            CLOG_INFO("created room");
            clvRoomDebugOutput(room);
            self->count++;
            *outSession = room;
            return i;
        }
    }
    *outSession = 0;
    return -1;
}

static inline int roomsFind(ClvRooms* self, size_t index, ClvRoom** outSession)
{
    if (index > self->capacity) {
        *outSession = 0;
        return -1;
    }

    *outSession = &self->rooms[index];

    return 0;
}

static inline int roomsFindByName(ClvRooms* self, const char* name, ClvRoom** outRoom)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        ClvRoom* room = &self->rooms[i];
        if (room->name && tc_str_equal(room->name, name)) {
            *outRoom = room;
            return 0;
        }
    }

    *outRoom = 0;

    return -1;
}

int clvRoomsReadAndFind(ClvRooms* self, FldInStream* stream, ClvRoom** outSession)
{
    uint32_t roomId;

    clvSerializeReadRoomId(stream, &roomId);
    int errorCode = roomsFind(self, roomId, outSession);
    if (errorCode < 0) {
        CLOG_WARN("couldn't find room %d", roomId);
        return errorCode;
    }

    CLOG_VERBOSE("Found room %d", roomId);
    return 0;
}

int clvRoomsReadNameAndFind(ClvRooms* self, FldInStream* stream, ClvRoom** outSession)
{
    char name[64];
    int errorCode = clvReadString(stream, name, 64);
    if (errorCode < 0) {
        return errorCode;
    }

    errorCode = roomsFindByName(self, name, outSession);
    if (errorCode < 0) {
        CLOG_WARN("couldn't find room with name '%s'", name);
        return errorCode;
    }

    return 0;
}

int clvRoomsReadAndFindRoomConnection(ClvRooms* self, FldInStream* stream, const struct ClvUserSession * requiredUserSession,
                                      struct ClvRoomConnection** outRoomConnection)
{
    ClvRoom* session;

    int errorCode = clvRoomsReadAndFind(self, stream, &session);
    if (errorCode < 0) {
        *outRoomConnection = 0;
        return errorCode;
    }

    ClvRoomConnections* connections = &session->roomConnections;
    uint8_t roomConnectionIndex;
    fldInStreamReadUInt8(stream, &roomConnectionIndex);

    if (roomConnectionIndex >= connections->capacityCount) {
        *outRoomConnection = 0;
        return -97;
    }

    *outRoomConnection = &connections->connections[roomConnectionIndex];
    if ((*outRoomConnection)->owner == 0) {
        *outRoomConnection = 0;
        CLOG_SOFT_ERROR("no owner for this connection %hhu", roomConnectionIndex);
        return -98;
    }

    if (requiredUserSession != (*outRoomConnection)->owner) {
        *outRoomConnection = 0;
        CLOG_SOFT_ERROR("not allowed to access this room connection ", requiredUserSession)
        return -97;
    }

    return 0;
}
