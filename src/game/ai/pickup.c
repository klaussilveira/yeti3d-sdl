#include "../../y3d/y3d_yeti.h"
#include "../../y3d/y3d_engine.h"
#include "../../y3d/y3d_draw.h"
#include "../../y3d/y3d_quake.h"
#include "../../y3d/y3d_sound.h"
#include "../models.h"
#include "../sprites.h"

/*
** Name: pickup_tick
** Desc: A standard pickup behaviour. Entity is placed at a choosen position
**       from the ground and bobs up & down.
*/
void pickup_tick(yeti_t *yeti, entity_t *e, int z)
{
    cell_t *cell = ENTITY_CELL(yeti, e);
    e->z = cell_bot(cell, e) + (fixsin(yeti->keyboard.tick << 12) >> 3) + z;
}
