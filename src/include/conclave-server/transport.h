/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_TRANSPORT_H
#define CONCLAVE_SERVER_TRANSPORT_H

#include <stddef.h>
#include <stdint.h>

#include <conclave-server/address.h>

typedef int (*clvServerSendFn)(void* self, const ClvAddress* address, const uint8_t* data, size_t size);
typedef struct ClvServerSendDatagram {
    clvServerSendFn send;
    void* self;
} ClvServerSendDatagram;

#endif
