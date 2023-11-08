/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_UNIQUE_ID_H
#define CONCLAVE_SERVER_UNIQUE_ID_H

#include <stdint.h>
#include <stddef.h>

typedef uint64_t ClvUniqueId;

ClvUniqueId clvGenerateUniqueIdFromIndex(size_t index);
size_t clvUniqueIdGetIndex(ClvUniqueId uniqueIndex);

#endif
