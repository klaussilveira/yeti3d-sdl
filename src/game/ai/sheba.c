#include "../../y3d/y3d_yeti.h"
#include "../../y3d/y3d_engine.h"
#include "../../y3d/y3d_draw.h"
#include "../../y3d/y3d_quake.h"
#include "../../y3d/y3d_sound.h"
#include "../models.h"
#include "../sprites.h"

void sheba_tick(yeti_t *yeti, entity_t *e)
{
    u16 *animation[] = {spr_sheba0, spr_sheba1, spr_sheba2, spr_sheba3};
    e->visual.data = animation[(yeti->keyboard.tick >> 2) & 3];
}

void sheba_init(yeti_t *yeti, entity_t *e)
{
    static_sprite_init(yeti, e, NULL, TRUE, i2f(2), 256, i2f(2), i2f(2));
    e->ontick = sheba_tick;
}
