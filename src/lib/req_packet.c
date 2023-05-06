/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-serialize/commands.h>
#include <conclave-serialize/serialize.h>
#include <conclave-server/req_packet.h>
#include <conclave-server/req_user_login.h>
#include <conclave-server/room.h>
#include <conclave-server/server.h>
#include <conclave-server/user.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>

int clvReqPacket(ClvServer* self, const ClvUserSession* userSession, FldInStream* inStream, ClvResponse* response)
{
    ClvRoomConnection* foundRoomConnection;

    int errorCode = clvRoomsReadAndFindRoomConnection(&self->rooms, inStream, userSession->user, &foundRoomConnection);
    if (errorCode < 0) {
        return errorCode;
    }

    uint8_t destinationConnectionIndex;
    int err = clvSerializeReadRoomConnectionIndex(inStream, &destinationConnectionIndex);
    if (err < 0) {
        return err;
    }

    if (foundRoomConnection->id == destinationConnectionIndex) {
        CLOG_SOFT_ERROR("can not send to self")
        return -4;
    }

    uint16_t packetOctetCount;
    fldInStreamReadUInt16(inStream, &packetOctetCount);

    ClvRoomConnection* foundDestinationConnection = clvRoomFindConnection(foundRoomConnection->ownedByRoom,
                                                                          destinationConnectionIndex);

    if (foundDestinationConnection == 0) {
        CLOG_SOFT_ERROR("could not find the destination index")
    }

    uint8_t buf[1200];

    FldOutStream outStream;
    fldOutStreamInit(&outStream, buf, 1200);

    clvSerializeWriteCommand(&outStream, clvSerializeCmdPacketToClient, "prefix");
    fldOutStreamWriteUInt8(&outStream, foundRoomConnection->id);
    fldOutStreamWriteOctets(&outStream, inStream->p, packetOctetCount);
    inStream->pos += packetOctetCount;
    inStream->p += packetOctetCount;

    ClvServerSendDatagram* sendDatagram = &response->sendDatagram;
    sendDatagram->send(sendDatagram->self, &foundDestinationConnection->owner->address, inStream->p, packetOctetCount);

    return 0;
}
