/*
** Name: Yeti3D
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


#include "../y3d/y3d_draw.h"
#include "../y3d/y3d_engine.h"
#include "../y3d/y3d_quake.h"
#include "../y3d/y3d_sound.h"

#include "entities.h"
#include "models.h"
#include "sprites.h"

/******************************************************************************/

#include "ai/static_model.c"
#include "ai/static_sprite.c"
#include "ai/sheba.c"
#include "ai/torch.c"
#include "ai/pickup.c"
#include "ai/boost.c"
#include "ai/quad.c"
#include "ai/teleporter.c"
#include "ai/bullet.c"
#include "ai/explode.c"
#include "ai/death.c"

/******************************************************************************/

void barrontree_init(yeti_t *yeti, entity_t *e)
{
#ifdef __GBA__
    static_sprite_init(yeti, e, spr_tree1, TRUE, 512, 512, 450, i2f(4));
#else
    static_model_init(yeti, e, model_barrontree, TRUE, 512);
#endif
}

void leafyplant_init(yeti_t *yeti, entity_t *e)
{
#ifdef __GBA__
#else
    static_model_init(yeti, e, model_leafyplant, TRUE, 128);
#endif
}

void cactus_init(yeti_t *yeti, entity_t *e)
{
#ifdef __GBA__
#else
    static_model_init(yeti, e, model_cactus, TRUE, 512);
#endif
}

void potplant_init(yeti_t *yeti, entity_t *e)
{
    static_model_init(yeti, e, model_potplant, FALSE, 128);
}

void pillar_init(yeti_t *yeti, entity_t *e)
{
    static_model_init(yeti, e, model_pillar, TRUE, i2f(4));
}

void overpass_init(yeti_t *yeti, entity_t *e)
{
    cell_t *cell = ENTITY_CELL(yeti, e);
    static_model_init(yeti, e, model_overpass, TRUE, 0);
    e->z = cell_bot(cell, e) + i2f(4);
    cell->eos = i2f(4);
    cell->hos = cell->eos - 64;
}

void bridge_init(yeti_t *yeti, entity_t *e)
{
    cell_t *cell = ENTITY_CELL(yeti, e);
    static_model_init(yeti, e, model_bridge, TRUE, 0);
    e->z = cell_bot(cell, e) + i2f(2);
    cell->eos = i2f(2);
    cell->hos = cell->eos - 64;
}

void emptypot_init(yeti_t *yeti, entity_t *e)
{
    static_model_init(yeti, e, model_emptypot, FALSE, 128);
}

void pinechair_init(yeti_t *yeti, entity_t *e)
{
    static_model_init(yeti, e, model_pinechair, FALSE, 128);
}

void crate_init(yeti_t *yeti, entity_t *e)
{
    static_model_init(yeti, e, model_crate, FALSE, 128);
    e->t = DEG_45;
}

/******************************************************************************/


