/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-serialize/serialize.h>
#include <conclave-server/room.h>
#include <conclave-server/room_connection.h>
#include <conclave-server/rooms.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <imprint/allocator.h>
#include <inttypes.h>

void clvRoomsInit(ClvRooms* self, struct ImprintAllocator* allocator, Clog log)
{
    self->log = log;
    self->capacity = 1024;
    self->rooms = IMPRINT_ALLOC_TYPE_COUNT(allocator, ClvRoom, self->capacity);
    tc_mem_clear_type_n(self->rooms, self->capacity);
    for (size_t i = 0; i < self->capacity; ++i) {
        self->rooms[i].id = (ClvSerializeRoomId) i;
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

int clvRoomsCreate(ClvRooms* self, const char* name, const struct ClvUserSession* requiredUserSession,
                   size_t maxNumberOfMembers, ClvRoom** outSession)
{
    for (size_t i = 1; i < self->capacity; ++i) {
        ClvRoom* room = &self->rooms[i];
        if (room->name == 0) {
            Clog roomLog;
            roomLog.config = self->log.config;
            tc_snprintf(room->prefix, 32, "%s/%zu", self->log.constantPrefix, i);
            roomLog.constantPrefix = room->prefix;
            clvRoomInit(room, i, name, requiredUserSession, maxNumberOfMembers, roomLog);
            CLOG_C_INFO(&self->log, "created room %zu", i)
#if defined CLOG_LOG_ENABLED
            clvRoomDebugOutput(room);
#endif
            self->count++;
            *outSession = room;
            return (int) i;
        }
    }
    *outSession = 0;
    return -1;
}

static inline int roomsFind(ClvRooms* self, ClvSerializeRoomId index, ClvRoom** outSession)
{
    if (index > self->capacity) {
        *outSession = 0;
        return -1;
    }

    *outSession = &self->rooms[index];

    return 0;
}

int clvRoomsReadAndFind(ClvRooms* self, FldInStream* stream, ClvRoom** outSession)
{
    ClvSerializeRoomId roomId;

    clvSerializeReadRoomId(stream, &roomId);
    int errorCode = roomsFind(self, roomId, outSession);
    if (errorCode < 0) {
        CLOG_C_WARN(&self->log, "couldn't find room %d", roomId)
        return errorCode;
    }

    return 0;
}

int clvRoomsReadAndFindRoomConnection(ClvRooms* self, FldInStream* stream,
                                      const struct ClvUserSession* requiredUserSession,
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
        CLOG_C_SOFT_ERROR(&self->log, "no owner for this connection %hhu", roomConnectionIndex)
        return -98;
    }

    if (requiredUserSession != (*outRoomConnection)->owner) {
        *outRoomConnection = 0;
        CLOG_C_SOFT_ERROR(&self->log, "not allowed to access this room connection %" PRIX64,
                          requiredUserSession->userSessionId)
        return -97;
    }

    return 0;
}
