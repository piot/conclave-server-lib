/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-room/room.h>
#include <conclave-serialize/commands.h>
#include <conclave-serialize/debug.h>
#include <conclave-serialize/server_out.h>
#include <conclave-server/req_list_rooms.h>
#include <conclave-server/req_ping.h>
#include <conclave-server/req_room_create.h>
#include <conclave-server/req_room_join.h>
#include <conclave-server/req_room_rejoin.h>
#include <conclave-server/req_user_login.h>
#include <conclave-server/rooms.h>
#include <conclave-server/server.h>
#include <conclave-server/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <guise-serialize/serialize.h>
#include <guise-sessions-client/user_session.h>

#include <secure-random/secure_random.h>

static int readAndLookupUserSession(GuiseSclClient* client, const GuiseSclAddress* address, FldInStream* inStream,
                                    const GuiseSclUserSession** out)
{
    GuiseSerializeUserSessionId userSessionId;
    int err = guiseSerializeReadUserSessionId(inStream, &userSessionId);
    if (err < 0) {
        *out = 0;
        return err;
    }

    err = guiseSclClientLookup(client, address, userSessionId, out);
    if (err < 0) {
        *out = 0;
        return err;
    }

    return 0;
}

int clvServerFeed(ClvServer* self, const GuiseSclAddress* address, const uint8_t* data, size_t len,
                  ClvResponse* response)
{
    CLOG_ASSERT(len != 0, "clvServerFeed: len is not allowed to be zero")
#if defined CLOG_LOG_ENABLED
    CLOG_C_VERBOSE(&self->log, "clvServerFeed: feed: %s octetCount: %zu", clvSerializeCmdToString(data[0]), len)
#endif
#define UDP_MAX_SIZE (1200)
    static uint8_t buf[UDP_MAX_SIZE];

#define BIG_BUFFER_SIZE (UDP_MAX_SIZE * 6)
    static uint8_t bigBuffer[BIG_BUFFER_SIZE];

    FldOutStream outStream;
    fldOutStreamInit(&outStream, buf, UDP_MAX_SIZE);
    int result = -1;

    FldInStream inStream;
    fldInStreamInit(&inStream, data + 1, len - 1);

    switch (data[0]) {
        case clvSerializeCmdLogin: {
            const GuiseSclUserSession* foundUserSession;
            int err = readAndLookupUserSession(&self->guiseSclClient, address, &inStream, &foundUserSession);
            if (err < 0) {
                if (err == -1) {
                    CLOG_C_VERBOSE(&self->log, "did not get a response from guise daemon yet")
                    return 0;
                }
                CLOG_C_WARN(&self->log, "something went wrong with guise scl client lookup %d", err)
                return err;
            }
            result = clvReqUserLogin(self, foundUserSession, &inStream, &outStream);
        } break;
        default: {
            ClvUserSession* foundUserSession;
            int err = clvUserSessionsReadAndFind(&self->userSessions, address, &inStream, &foundUserSession);
            if (err < 0) {
                return err;
            }
            switch (data[0]) {
                case clvSerializeCmdRoomCreate: {
                    MonotonicTimeMs now = monotonicTimeMsNow();
                    result = clvReqRoomCreate(&self->rooms, foundUserSession, now, &inStream, &outStream);
                } break;
                case clvSerializeCmdRoomJoin: {
                    MonotonicTimeMs now = monotonicTimeMsNow();
                    result = clvReqRoomJoin(self, foundUserSession, now, &inStream, &outStream);
                } break;
                case clvSerializeCmdRoomReJoin:
                    result = clvReqRoomReJoin(self, (const ClvUserSession*) foundUserSession, &inStream, &outStream);
                    break;
                case clvSerializeCmdListRooms: {
                    FldOutStream bigStream;
                    fldOutStreamInit(&bigStream, bigBuffer, BIG_BUFFER_SIZE);

                    result = clvReqListRooms(self, (const ClvUserSession*) foundUserSession, &inStream, &bigStream);
                    datagramReassemblyWriteNextSequence(&foundUserSession->write);
                    const int chunkSize = 1000;
                    size_t chunkCount = bigStream.pos / chunkSize;
                    uint16_t last = bigStream.pos % chunkSize;

                    for (size_t i = 0; i < chunkCount; ++i) {
                        fldOutStreamRewind(&outStream);
                        clvSerializeWriteCommand(&outStream, clvSerializeCmdListRoomsResponse, "");
                        // clvSerializeWriteUserSessionId(&outStream, foundUserSession->userSessionId);
                        datagramReassemblyWriteHeader(&foundUserSession->write, &outStream, chunkSize,
                                                      last == 0 && i == chunkCount - 1);
                        uint8_t* p = bigStream.octets + i * chunkSize;
                        fldOutStreamWriteOctets(&outStream, p, chunkSize);
                        response->transportOut->send(response->transportOut->self, outStream.octets, outStream.pos);
                    }

                    if (last > 0) {
                        fldOutStreamRewind(&outStream);
                        clvSerializeWriteCommand(&outStream, clvSerializeCmdListRoomsResponse, "");
                        // clvSerializeWriteUserSessionId(&outStream, foundUserSession->userSessionId);
                        datagramReassemblyWriteHeader(&foundUserSession->write, &outStream, last, true);
                        uint8_t* p = bigStream.octets + chunkCount * chunkSize;
                        fldOutStreamWriteOctets(&outStream, p, chunkSize);
                        response->transportOut->send(response->transportOut->self, outStream.octets, outStream.pos);
                    }

                    return 0;
                }
                case clvSerializeCmdPing: {
                    MonotonicTimeMs now = monotonicTimeMsNow();
                    result = clvReqPing(self, (const ClvUserSession*) foundUserSession, now, &inStream, &outStream);
                } break;
                default:
                    CLOG_C_SOFT_ERROR(&self->log, "clvServerFeed: unknown command %02X", data[0])
                    return 0;
            }
            break;
        }
    }

    if (result < 0) {
        return result;
    }

    return response->transportOut->send(response->transportOut->self, buf, outStream.pos);
}

int clvServerInit(ClvServer* self, DatagramTransport transportToGuiseServer,
                  GuiseSerializeUserSessionId assignedSessionIdForThisRelayServer, struct ImprintAllocator* memory,
                  Clog log)
{
    self->log = log;

    Clog subLog;
    subLog.config = log.config;
    guiseSclClientInit(&self->guiseSclClient, transportToGuiseServer, assignedSessionIdForThisRelayServer, subLog);

    tc_snprintf(self->rooms.prefix, 32, "%s/rooms", log.constantPrefix);
    subLog.constantPrefix = self->rooms.prefix;
    clvRoomsInit(&self->rooms, memory, subLog);

    tc_snprintf(self->userSessions.prefix, 32, "%s/usersessions", log.constantPrefix);
    subLog.constantPrefix = self->userSessions.prefix;
    clvUserSessionsInit(&self->userSessions, subLog);

    return 0;
}

int clvServerUpdate(ClvServer* self)
{
    return guiseSclClientUpdate(&self->guiseSclClient);
}

void clvServerReset(ClvServer* self)
{
    clvRoomsReset(&self->rooms);
    clvUserSessionsReset(&self->userSessions);
}

void clvServerDestroy(ClvServer* self)
{
    clvRoomsDestroy(&self->rooms);
    clvUserSessionsDestroy(&self->userSessions);
}
