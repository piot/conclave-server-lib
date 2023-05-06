/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_CONNECTION_H
#define CONCLAVE_SERVER_ROOM_CONNECTION_H

#include <stdint.h>
#include <stddef.h>

struct ClvRoomMember;
struct ClvUserSession;
struct ImprintAllocator;

/** Represents a unreliable datagram transport "connection" from a client */
typedef struct ClvRoomConnection {
    uint32_t id;
    size_t memberCount;
    struct ClvRoomMember** members;
    const struct ClvUserSession* owner;
} ClvRoomConnection;

void clvRoomConnectionInit(ClvRoomConnection* self, struct ImprintAllocator* allocator, size_t maxOctets,
                           struct ClvRoomMember** members, size_t memberCount);

int clvRoomConnectionHasMemberId(const ClvRoomConnection* self, uint8_t memberId);

#endif
