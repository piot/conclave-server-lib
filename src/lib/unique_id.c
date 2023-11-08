/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <conclave-server/unique_id.h>
#include <stdlib.h>

ClvUniqueId clvGenerateUniqueIdFromIndex(size_t index)
{
    uint32_t upperPart = (((uint32_t)rand()) % 0xffffffff);

    ClvUniqueId uniqueIndex = ((uint64_t)upperPart << 32) | index;

    return uniqueIndex;
}

size_t clvUniqueIdGetIndex(ClvUniqueId uniqueIndex)
{
    return uniqueIndex & 0xffffffff;
}
