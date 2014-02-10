#include "../../y3d/y3d_yeti.h"
#include "../../y3d/y3d_engine.h"
#include "../../y3d/y3d_draw.h"
#include "../models.h"
#include "../sprites.h"

void bullet_hit(yeti_t *yeti, entity_t *e1, entity_t *e2)
{
}

void bullet_wall(yeti_t *yeti, entity_t *e, cell_t *c)
{
    explode_init(yeti, e);
}

void bullet_tick(yeti_t *yeti, entity_t *e)
{
    if (!e->xx && !e->yy) {
        explode_init(yeti, e);
    }
}

void bullet_shoot_at(entity_t *e, int x, int y, int z)
{
    entity_look_at2(e, x, y, z);
    entity_set_velocity(e);
}

void bullet_shoot(yeti_t *yeti, entity_t *e)
{
    entity_t *b = YETI_BULLET(yeti);
    entity_destroy(b);
    b->t  = e->t;
    b->p  = -e->p >> 1;
    b->x  = e->x + (b->xx >> 1);
    b->y  = e->y + (b->yy >> 1);
    b->z  = e->z - 100;
    entity_set_velocity(b);
    b->visual.data    = spr_flare0;
    b->visual.width   = 200;
    b->visual.height  = 200;
    b->visual.mode    = DRAW_MODE_COLORKEY_BLEND;
    b->radius = 10;
    b->ontick = bullet_tick;
    b->onwall = bullet_wall;
    b->onhit  = bullet_hit;
    b->emitlight = TRUE;
}
