/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-server/room_member.h>
#include <conclave-server/room_members.h>

int clvRoomMembersJoin(ClvRoomMembers* self, const ClvMemberJoinInfo* joinInfo, size_t localParticipantCount,
                        ClvRoomMember** results)
{
    if (self->participantCount + localParticipantCount > self->participantCapacity) {
        CLOG_WARN("couldn't join, session is full")
        return -2;
    }
    size_t joinIndex = 0;

    for (size_t i = 0; i < self->participantCapacity; ++i) {
        struct ClvRoomMember* participant = &self->participants[i];
        if (participant->name == 0) {
            const ClvMemberJoinInfo* joiner = &joinInfo[joinIndex];
            participant->name = tc_str_dup(joiner->name);
            participant->localIndex = joiner->localIndex;
            participant->id = ++self->lastUniqueId;
            results[joinIndex] = participant;
            joinIndex++;
            if (joinIndex == localParticipantCount) {
                break;
            }
        }
    }

    if (joinIndex != localParticipantCount) {
        return -2;
    }

    return 0;
}

void clvRoomMembersInit(ClvRoomMembers* self, size_t maxCount)
{
    self->participantCapacity = maxCount;
    self->participants = tc_malloc_type_count(ClvRoomMember, maxCount);
    tc_mem_clear_type_n(self->participants, maxCount);
    self->participantCount = 0;
    self->lastUniqueId = 0;
}

void clvRoomMembersDestroy(ClvRoomMembers* self)
{
    tc_free(self->participants);
}
