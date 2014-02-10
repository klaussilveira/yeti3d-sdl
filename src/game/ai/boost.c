#include "../../y3d/y3d_yeti.h"
#include "../../y3d/y3d_engine.h"
#include "../models.h"

void boost_tick(yeti_t *yeti, entity_t *e)
{
    pickup_tick(yeti, e, 256);
}

void boost_init(yeti_t *yeti, entity_t *e)
{
    e->tt = i2f(8);
    e->visual.data = model_boost;
    e->visual.ondraw = yeti_model_draw;
    e->ontick = boost_tick;
    //e->onhit  = guard_hit;
}
