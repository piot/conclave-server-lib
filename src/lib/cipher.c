/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "cipher.h"

uint64_t extremelyUnsecureCipher(uint64_t publicKey, uint64_t secretKey)
{
    return publicKey ^ secretKey;
}
