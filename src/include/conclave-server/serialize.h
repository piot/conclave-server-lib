/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_SERIALIZE2_H
#define CONCLAVE_SERVER_SERIALIZE2_H

#include <stddef.h>

struct FldInStream;

int clvReadString(struct FldInStream* stream, char* buf, size_t maxLength);

#endif
