/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <conclave-server/serialize.h>
#include <flood/in_stream.h>
#include <stdint.h>

int clvReadString(struct FldInStream* stream, char* buf, size_t maxLength)
{
    uint8_t stringLength;
    fldInStreamReadUInt8(stream, &stringLength);
    if (stringLength + 1 > maxLength) {
        return -1;
    }
    int errorCode = fldInStreamReadOctets(stream, (uint8_t*) buf, stringLength);
    if (errorCode < 0) {
        return errorCode;
    }
    buf[stringLength] = 0;
    return stringLength;
}
