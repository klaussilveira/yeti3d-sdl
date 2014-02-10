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

#include "y3d_cell.h"
#include "y3d_fixed.h"
#include "y3d_texture.h"

/*
** Name: cell_init
** Desc: Initialize a yeti cell to its defaults.
*/
void cell_init(cell_t *cell, int issolid, int islight)
{
    CLEARMEM(cell);
    cell->top = issolid ? i2f(0) : i2f(4);
    cell->bot = i2f(0);
#ifndef __GBA__
    cell->tsa = cell->top;
    cell->bsa = cell->bot;
#endif
    cell->lit = CELL_LIGHT_MAX;
    cell->ttx = TEXTURE_TOP;
    cell->wtt = cell->wtb = TEXTURE_WALL;
    cell->btx = TEXTURE_BOT;

    if (islight) {
        cell->swi |= CELL_SWI_LIGHT;
    }
}

/*
** Name: cells_are_equal
** Desc: Returns TRUE is two cells have equal settings. Some values may
**       be checked using a threshold. eg: lighting.
*/
int cells_are_equal(cell_t *c1, cell_t *c2)
{
    return
        c1->tops[0]   == c2->tops[0] &&
        c1->tops[1]   == c2->tops[1] &&
        c1->tops[2]   == c2->tops[2] &&
        c1->tops[3]   == c2->tops[3] &&
        c1->bots[0]   == c2->bots[0] &&
        c1->bots[1]   == c2->bots[1] &&
        c1->bots[2]   == c2->bots[2] &&
        c1->bots[3]   == c2->bots[3] &&
        c1->wwi       == c2->wwi     &&
        c1->wtb       == c2->wtb     &&
        c1->wtt       == c2->wtt     &&
        c1->ttx       == c2->ttx     &&
        c1->btx       == c2->btx     &&
        CELL_LUA(c1)  == CELL_LUA(c2);
}



