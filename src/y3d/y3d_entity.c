/*
** Name: Yeti3D PRO
** Desc: Portable GameBoy Advanced 3D Engine
** Auth: Derek J. Evans <derek@theteahouse.com.au>
**
** Copyright (C) 2003-2004 Derek J. Evans. All Rights Reserved.
**
** YY  YY EEEEEE TTTTTT IIIIII 33333  DDDDD
** YY  YY EE       TT     II       33 DD  DD
**  YYYY  EEEE     TT     II     333  DD  DD
**   YY   EE       TT     II       33 DD  DD
**   YY   EEEEEE   TT   IIIIII 33333  DDDDD
*/

#include "y3d_entity.h"
#include "y3d_fixed.h"
#include "y3d_draw.h"

/******************************************************************************/

/*
** Name: entity_visual_init
** Desc:
*/
void entity_visual_init(entity_visual_t *entity_visual)
{
    CLEARMEM(entity_visual);
    entity_visual->mode = DRAW_MODE_COPY;
}

/******************************************************************************/

/*
** Name: entity_destroy
** Desc:
*/
void entity_destroy(entity_t *e)
{
    CLEARMEM(e);
}

/*
** Name: entity_init
** Desc:
*/
void entity_init(entity_t *e, int x, int y, int z)
{
    entity_destroy(e);
    e->x = x;
    e->y = y;
    e->z = z;
    entity_visual_init(&e->visual);
}

/*
** Name: entity_reinit
** Desc: Initilize a entity without moving its position. Used for when
**       you want to change an entity to another type.
*/
void entity_reinit(entity_t *e)
{
    entity_init(e, e->x, e->y, e->z);
}

/*
** Name: entity_freeze
** Desc: Zeros all entity velocities. Used for pain & death AI.
*/
void entity_freeze(entity_t *e)
{
    e->xx = e->yy = e->zz = e->tt = e->rr = e->pp = 0;
}

/******************************************************************************/

void entity_friction(entity_t *e, int amount)
{
    e->xx = ansic_friction(e->xx, amount);
    e->yy = ansic_friction(e->yy, amount);
}

void entity_move_forward(entity_t *e)
{
    e->xx += fixsin(e->t) >> 4;
    e->yy += fixcos(e->t) >> 4;
}

void entity_move_backwards(entity_t *e)
{
    e->xx -= fixsin(e->t) >> 4;
    e->yy -= fixcos(e->t) >> 4;
}

void entity_turn_right(entity_t *e)
{
    e->tt += i2f(3);
}

void entity_turn_left(entity_t *e)
{
    e->tt -= i2f(3);
}

void entity_turn_towards(entity_t *e, int x, int y)
{
    int x1 = x - (e->x + fixsin(e->t + i2f(14)));
    int y1 = y - (e->y + fixcos(e->t + i2f(14)));
    int x2 = x - (e->x + fixsin(e->t - i2f(14)));
    int y2 = y - (e->y + fixcos(e->t - i2f(14)));

    if ((x1 * x1 + y1 * y1) < (x2 * x2 + y2 * y2)) {
        entity_turn_right(e);
    } else {
        entity_turn_left(e);
    }
}

/*
** Name: entity_look_at
** Desc: I think this is the old version. Doesn't use a sqrt, but doesn't calc
**       the pitch correctly.
*/
void entity_look_at(entity_t *e, int x, int y, int z)
{
    x = x - e->x;
    y = y - e->y;
    z = e->z - z;
    e->t = fixangle(x, y);
    e->p = fixangle(ABS(z), ABS(y));

    if (e->p > i2f(32)) {
        e->p = i2f(32);
    }
}

/*
** Name: entity_look_at2
** Desc: Setup a entities turn and pitch so it points towards a given
**       point.
*/
void entity_look_at2(entity_t *e, int x, int y, int z)
{
    x -= e->x;
    y -= e->y;
    z -= e->z;
    e->t = fixangle(x, y);
    e->p = fixangle(-z, isqrt(x * x + y * y));
}

/*
** Name: entity_set_velocity
** Desc: Setup a entities velocity based on its turn & pitch.
*/
void entity_set_velocity(entity_t *e)
{
    e->xx = fixsin(e->t);
    e->yy = fixcos(e->t);
    e->zz = fixsin(e->p);
}

void entity_force_towards(entity_t *e, int x, int y, int z, int shift)
{
    e->xx = (x - e->x) >> shift;
    e->yy = (y - e->y) >> shift;
    e->zz = (z - e->z) >> shift;
}

/******************************************************************************/


