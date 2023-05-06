/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-server/room_member.h>
#include <conclave-server/room_members.h>

int clvRoomMembersJoin(ClvRoomMembers* self, const ClvMemberJoinInfo* joinInfo, size_t localMemberCount,
                        ClvRoomMember** results)
{
    if (self->memberCount + localMemberCount > self->memberCapacity) {
        CLOG_WARN("couldn't join, session is full")
        return -2;
    }
    size_t joinIndex = 0;

    for (size_t i = 0; i < self->memberCapacity; ++i) {
        struct ClvRoomMember* member = &self->members[i];
        if (member->name == 0) {
            const ClvMemberJoinInfo* joiner = &joinInfo[joinIndex];
            member->name = tc_str_dup(joiner->name);
            member->localIndex = joiner->localIndex;
            member->id = ++self->lastUniqueId;
            results[joinIndex] = member;
            joinIndex++;
            if (joinIndex == localMemberCount) {
                break;
            }
        }
    }

    if (joinIndex != localMemberCount) {
        return -2;
    }

    return 0;
}

void clvRoomMembersInit(ClvRoomMembers* self, size_t maxCount)
{
    self->memberCapacity = maxCount;
    self->members = tc_malloc_type_count(ClvRoomMember, maxCount);
    tc_mem_clear_type_n(self->members, maxCount);
    self->memberCount = 0;
    self->lastUniqueId = 0;
}

void clvRoomMembersDestroy(ClvRoomMembers* self)
{
    tc_free(self->members);
}
