/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <conclave-server/room_connection.h>
#include <conclave-server/room_member.h>
#include <imprint/allocator.h>
#include <tiny-libc/tiny_libc.h>

/**
 * Initializes a member connection.
 * @param self the member connection
 * @param maxOctets max number of octets to be used for the steps buffer.
 * @param stepId starting StepId for this connection. Game creator will have 0, but later
 * joiners can have another number.
 * @param members The members to associate with this connection.
 * @param memberCount number of members in \p members.
 *
 * Need to create members before associating them to the connection.
 */
void clvRoomConnectionInit(ClvRoomConnection* self, ImprintAllocator* connectionAllocator, size_t maxOctets,
                           struct ClvRoomMember** members, size_t memberCount)
{
    self->members = IMPRINT_ALLOC_TYPE_COUNT(connectionAllocator, ClvRoomMember*, memberCount);
    tc_memcpy_octets(self->members, members, memberCount * sizeof(ClvRoomMember*));
    self->memberCount = memberCount;
}

int clvRoomConnectionHasMemberId(const ClvRoomConnection* self, uint8_t memberId)
{
    for (size_t i = 0; i < self->memberCount; ++i) {
        const ClvRoomMember* member = self->members[i];
        if (member->id == memberId) {
            return 1;
        }
    }

    return 0;
}