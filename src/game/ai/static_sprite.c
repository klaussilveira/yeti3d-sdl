#include "../../y3d/y3d_yeti.h"
#include "../../y3d/y3d_engine.h"
#include "../../y3d/y3d_draw.h"
#include "../sprites.h"

/*
** Name: static_sprite_init
** Desc: Initilize a static 2D sprite.
*/
void static_sprite_init(yeti_t *yeti, entity_t *e, void *sprite, int castshadow, int eos, int z, int width, int height)
{
    cell_t *cell = ENTITY_CELL(yeti, e);
    e->visual.width = width;
    e->visual.height = height;
    e->visual.data = sprite;
    e->visual.mode = DRAW_MODE_COLORKEY;
    e->z = cell_bot(cell, e) + z;
    e->castshadow = castshadow;
    cell->eos = eos;
}
