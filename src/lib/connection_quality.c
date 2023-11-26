/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <conclave-server/connection_quality.h>

void clvConnectionQualityInit(ClvConnectionQuality* self, MonotonicTimeMs now)
{
    statsIntPerSecondInit(&self->pingsPerSecond, now, 1000);
}

int clvConnectionQualityUpdate(ClvConnectionQuality* self, MonotonicTimeMs now)
{
    int status = statsIntPerSecondUpdate(&self->pingsPerSecond, now);
    CLOG_VERBOSE("quality: pingPerSecond: %d isSet: %d", self->pingsPerSecond.avg, self->pingsPerSecond.avgIsSet)
    return status;
}

/// checks if the connection has been decided to be disconnected
/// @param self connection quality
/// @return true if the connection should be disconnected
bool clvConnectionQualityShouldDisconnect(const ClvConnectionQuality* self)
{
    return self->pingsPerSecond.avgIsSet && self->pingsPerSecond.avg <= 1;
}

void clvConnectionQualityOnPing(ClvConnectionQuality* self, MonotonicTimeMs now)
{
    self->lastPingAt = now;
    statsIntPerSecondAdd(&self->pingsPerSecond, 2); // TODO: SHOULD BE 1
}
