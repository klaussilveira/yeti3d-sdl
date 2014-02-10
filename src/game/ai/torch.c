#include "../../y3d/y3d_yeti.h"
#include "../../y3d/y3d_engine.h"
#include "../../y3d/y3d_draw.h"
#include "../../y3d/y3d_quake.h"
#include "../../y3d/y3d_sound.h"
#include "../models.h"
#include "../sprites.h"

void torch0_tick(yeti_t *yeti, entity_t *e)
{
    u16 *animation[] = {spr_torch0, spr_torch1, spr_torch2, spr_torch3};
    e->visual.data = animation[(yeti->keyboard.tick >> 2) & 3];
}

void torch0_init(yeti_t *yeti, entity_t *e)
{
    cell_t *cell = ENTITY_CELL(yeti, e);
    e->visual.width = i2f(2);
    e->visual.height = i2f(2);
    e->visual.mode = DRAW_MODE_COLORKEY;
    e->radius = 0;
    e->ontick = torch0_tick;
    e->z = cell_bot(cell, e) + 128;
    cell->eos = i2f(1);
}

void torch1_tick(yeti_t *yeti, entity_t *e)
{
    u16 *animation[] = {spr_torch4, spr_torch5, spr_torch6, spr_torch7};
    e->visual.data = animation[(yeti->keyboard.tick >> 2) & 3];
}

void torch1_init(yeti_t *yeti, entity_t *e)
{
    cell_t *cell = ENTITY_CELL(yeti, e);
    e->visual.width = i2f(3);
    e->visual.height = i2f(3);
    e->visual.mode = DRAW_MODE_COLORKEY;
    e->radius = 0;
    e->ontick = torch1_tick;
    e->z = cell_bot(cell, e) + 240;
    cell->eos = i2f(1);
}
