/*----------------------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved. https://github.com/piot/conclave-server-lib
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------------------*/
#include <conclave-server/room_connection.h>

/**
 * Initializes a member connection.
 * @param self the member connection
 */
void clvRoomConnectionInit(ClvRoomConnection* self, struct ClvRoom* room, const struct ClvUserSession* userSession)
{
    self->owner = userSession;
    self->ownedByRoom = room;
}
