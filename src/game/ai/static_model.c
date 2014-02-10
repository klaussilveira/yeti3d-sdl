#include "../../y3d/y3d_yeti.h"
#include "../../y3d/y3d_engine.h"
#include "../models.h"

/*
** Name: static_model_init
** Desc: Initilize a static 3D model.
*/
void static_model_init(yeti_t *yeti, entity_t *e, void *model, int castshadow, int eos)
{
    cell_t *cell = ENTITY_CELL(yeti, e);
    e->visual.data = model;
    e->visual.ondraw = yeti_model_draw;
    e->z = cell_bot(cell, e);
    e->castshadow = castshadow;
    cell->eos = eos;
}
