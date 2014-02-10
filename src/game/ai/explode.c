#include "../../y3d/y3d_yeti.h"
#include "../../y3d/y3d_engine.h"
#include "../../y3d/y3d_draw.h"
#include "../../y3d/y3d_quake.h"
#include "../../y3d/y3d_sound.h"
#include "../models.h"
#include "../sprites.h"

void explode_tick(yeti_t *yeti, entity_t *e)
{
    u16 *animation[] = {
        spr_expl0, spr_expl1, spr_expl2, spr_expl3,
        spr_expl4, spr_expl5, spr_expl6, spr_expl7,
        spr_expl8, spr_expl9, spr_expl10, spr_expl11,
        spr_expl12, spr_expl13, spr_expl14, spr_expl15
    };
    e->visual.data = animation[f2i(e->animation.pos)];
    animation_tick(&e->animation);

    if (e->animation.nloops) {
        e->visual.width += 48;
        e->visual.height += 48;
    } else {
        entity_destroy(e);
    }
}

void explode_init(yeti_t *yeti, entity_t *e)
{
    e->x -= e->xx;
    e->y -= e->yy;
    e->z -= e->zz;
    entity_reinit(e);
    e->radius = 0;
    e->visual.width = 16;
    e->visual.height = 16;
    e->visual.mode = DRAW_MODE_COLORKEY_BLEND;
    e->ontick = explode_tick;
    e->onhit = 0;
    e->onwall = 0;
    sound_play(SOUND_EXPLOSION);
    animation_init(&e->animation, 0, 0, 16, 200, 1);
}


void splat_tick(yeti_t *yeti, entity_t *e)
{
    u16 *animation[] = {
        spr_splat00, spr_splat01, spr_splat02, spr_splat03,
        spr_splat04, spr_splat05, spr_splat06, spr_splat07,
        spr_splat08, spr_splat09, spr_splat10, spr_splat11,
        spr_splat12, spr_splat13, spr_splat14, spr_splat15,
        spr_splat16, spr_splat17, spr_splat18, spr_splat19,
        spr_splat20, spr_splat21, spr_splat22, spr_splat23,
        spr_splat24, spr_splat25, spr_splat26, spr_splat27,
        spr_splat28, spr_splat29, spr_splat30, spr_splat31
    };
    e->visual.data = animation[f2i(e->animation.pos)];
    animation_tick(&e->animation);

    if (e->animation.nloops) {
        e->visual.width += 32;
        e->visual.height += 32;
    } else {
        entity_destroy(e);
    }
}

void splat_init(yeti_t *yeti, entity_t *e)
{
    entity_reinit(e);
    e->radius = 0;
    e->visual.width = 32;
    e->visual.height = 32;
    e->visual.mode = DRAW_MODE_COLORKEY;
    e->ontick = splat_tick;
    e->onhit = 0;
    e->onwall = 0;
    sound_play(SOUND_EXPLOSION);
    animation_init(&e->animation, 0, 0, 32, 300, LOOP_ONCE);
}
