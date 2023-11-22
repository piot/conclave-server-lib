/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#ifndef CONCLAVE_SERVER_CONNECTION_QUALITY_H
#define CONCLAVE_SERVER_CONNECTION_QUALITY_H

#include <stats/stats_per_second.h>

typedef struct ClvConnectionQuality {
    MonotonicTimeMs lastPingAt;
    StatsIntPerSecond pingsPerSecond;
} ClvConnectionQuality;

void clvConnectionQualityInit(ClvConnectionQuality* self, MonotonicTimeMs now);
int clvConnectionQualityUpdate(ClvConnectionQuality* self, MonotonicTimeMs now);
void clvConnectionQualityOnPing(ClvConnectionQuality* self, MonotonicTimeMs now);
bool clvConnectionQualityShouldDisconnect(const ClvConnectionQuality* self);

#endif
