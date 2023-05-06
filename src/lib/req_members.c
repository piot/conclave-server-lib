/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-server/room.h>
#include <conclave-server/room_connection.h>
#include <conclave-server/room_join.h>
#include <conclave-server/serialize.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <tiny-libc/tiny_libc.h>

int clvReadAndJoinMembers(ClvRoom* session, ClvUserSession* ownerOfConnection, struct FldInStream* inStream,
                          ClvRoomConnection** createdConnection)
{
    uint8_t localMemberCount;
    fldInStreamReadUInt8(inStream, &localMemberCount);
    CLOG_ASSERT(localMemberCount > 0, "must have local members")
    ClvMemberJoinInfo joinInfos[8];
    char tempName[64];
    for (size_t i = 0; i < localMemberCount; ++i) {
        fldInStreamReadUInt8(inStream, &joinInfos[i].localIndex);
        int lengthCount = clvReadString(inStream, tempName, 64);
        if (lengthCount < 0) {
            *createdConnection = 0;
            return lengthCount;
        }
        joinInfos[i].name = tc_str_dup(tempName);
    }
    int errorCode = clvRoomCreateRoomConnection(session, ownerOfConnection, joinInfos, localMemberCount,
                                                createdConnection);
    if (errorCode < 0) {
        CLOG_WARN("couldn't join game session")
        return errorCode;
    }
    return 0;
}