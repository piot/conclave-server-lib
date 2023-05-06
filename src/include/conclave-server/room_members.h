/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_ROOM_MEMBERS_H
#define CONCLAVE_SERVER_ROOM_MEMBERS_H

#include <stdint.h>
#include <stdlib.h>

struct ClvRoomMember;

typedef struct ClvRoomMembers {
    struct ClvRoomMember* participants;
    size_t participantCapacity;
    size_t participantCount;
    uint8_t lastUniqueId;
} ClvRoomMembers;

typedef struct ClvMemberJoinInfo {
    uint8_t localIndex;
    const char* name;
} ClvMemberJoinInfo;

void clvRoomMembersInit(ClvRoomMembers* self, size_t maxCount);
void clvRoomMembersDestroy(ClvRoomMembers* self);
int clvRoomMembersJoin(ClvRoomMembers* self, const ClvMemberJoinInfo* joinInfo, size_t localParticipantCount,
                        struct ClvRoomMember** results);

#endif
