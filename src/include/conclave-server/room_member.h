/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_MEMBER_H
#define CONCLAVE_SERVER_ROOM_MEMBER_H

#include <stdlib.h>

typedef struct ClvRoomMember {
    size_t localIndex;
    size_t id;
    const char* name;
    size_t participantIndex;
} ClvRoomMember;

#endif
