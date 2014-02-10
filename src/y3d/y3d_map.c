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

#include "y3d_map.h"
#include "y3d_fixed.h"
#include "y3d_texture.h"

/*
** Name: map_init
** Desc: Clears a yeti map to its default settings.
*/
void map_init(map_t *map, int lod)
{
    int x, y;
    CLEARMEM(map);
    map->width  = MAP_WIDTH;
    map->height = MAP_HEIGHT;

    for (y = map->height; y--;) {
        for (x = map->width; x--;) {
            cell_init(&map->item[y][x], MAP_IS_EDGE(x, y), !((x + 8) & 15) && !((y + 8) & 15));
        }
    }

    map_precalc_data(map, lod);
}

/*
** Name: map_ungroup
** Desc: Ungroups a block of cells.
*/
void map_ungroup(map_t *map, int x1, int y1, int x2, int y2)
{
    int x, y;

    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            map->item[y][x].xlod = map->item[y][x].ylod = 1;
        }
    }
}

/*
** Name: map_group
** Desc: Groups blocks of cells together. Used for recording
**       level of detail data.
*/
void map_group(map_t *map, int x1, int y1, int x2, int y2)
{
    int x, y;

    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            map->item[y][x].xlod = x1 - x;
            map->item[y][x].ylod = y1 - y;
        }
    }

    map->item[y1][x1].xlod = x2 - x1;
    map->item[y1][x1].ylod = y2 - y1;
}

/*
** Name: map_cells_are_equal
** Desc:
*/
int map_cells_are_equal(map_t *map, int x1, int y1, int x2, int y2)
{
    int x, y;

    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            if (!cells_are_equal(&map->item[y1][x1], &map->item[y][x])) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/*
** Name: map_cells_are_group
** Desc: Returns TRUE if a block of cells can be grouped. Otherwise FALSE.
*/
int map_cells_are_group(map_t *map, int x1, int y1, int x2, int y2)
{
    int x, y;

    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            cell_t *cell = &map->item[y][x];

            if (
                cell->xlod != 1 ||
                cell->ylod != 1 ||
                cell->swi & CELL_SWI_WARPED ||
                TEXTURE_IS_LIQUID(cell->btx)
            ) {
                return FALSE;
            }
        }
    }

    return
        map_cells_are_equal(map, x1, y1, x2, y2) &&
        map_cells_are_equal(map, x1, y1 - 1, x2, y1) &&
        map_cells_are_equal(map, x1, y2, x2, y2 + 1) &&
        map_cells_are_equal(map, x1 - 1, y1, x1, y2) &&
        map_cells_are_equal(map, x2, y1, x2 + 1, y2);
}

/******************************************************************************/

/*
** Name: map_init_lod
** Desc:
*/
void map_init_lod(map_t *map, int groupsize)
{
    int size, newsize, x1, y1, x2, y2;
    rect_t r;
    map_ungroup(map, MAP_RECT);

    for (y1 = 1; y1 < map->height - 1; y1++) {
        for (x1 = 1; x1 < map->width - 1; x1++) {
            size = r.x1 = r.y1 = r.x2 = r.y2 = 0;

            for (y2 = y1; y2 < (map->height - 1) && y2 <= (y1 + groupsize); y2++) {
                for (x2 = x1; x2 < (map->width - 1) && x2 <= (x1 + groupsize); x2++) {
                    if (map_cells_are_group(map, x1, y1, x2, y2)) {
                        newsize = (x2 - x1) * (y2 - y1);

                        if (newsize > size) {
                            r.x1 = x1;
                            r.y1 = y1;
                            r.x2 = x2;
                            r.y2 = y2;
                            size = newsize;
                        }
                    }
                }
            }

            if (size > 0) {
                map_group(map, r.x1, r.y1, r.x2, r.y2);
            }
        }
    }
}

/******************************************************************************/

/*
** Name: map_draw_light
** Desc: Renders a spot light at a given cell location. Light position is
**       expressed as 24:8 fixed point integers.
*/
void map_draw_light(map_t *map, int lightx, int lighty)
{
    int i, lit, xhit, yhit;
    cell_t *cell;

    for (i = 0; i < DEG_360; i += DEG_1) {
        int x = lightx, xx = fixsin(i) >> 1;
        int y = lighty, yy = fixcos(i) >> 1;

        for (lit = 64; lit > 0;) {
            x += xx;

            if ((xhit = CELL_IS_SOLID(&map->item[f2i(y)][f2i(x)])) != FALSE) {
                xx = -xx;
            }

            y += yy;

            if ((yhit = CELL_IS_SOLID(&map->item[f2i(y)][f2i(x)])) != FALSE) {
                yy = -yy;
            }

            cell = &map->item[f2i(y + FIXHALF)][f2i(x + FIXHALF)];
            cell->lit = MIN(cell->lit + lit, CELL_LIGHT_MAX);

            if (xhit || yhit) {
                break;    //lit >>= 2;
            }
        }
    }
}

/*
** Name: map_ambient_lit
** Desc: Setup a standard ambient lighting. All current lighting is removed.
*/
void map_ambient_lit(map_t *map, int lit)
{
    int x, y;

    for (y = map->height; y--;) {
        for (x = map->width; x--;) {
            map->item[y][x].lit = lit;
        }
    }
}

/******************************************************************************/

/*
** Name: map_smooth_lit
** Desc: Applies a smoothing effect to the map lighting. This removes harsh
**       light changes while simulating a radiant lighting. Adjust the
**       the smoothing to your own tastes.
*/
void map_smooth_lit(map_t *map, int x1, int y1, int x2, int y2)
{
    int x, y;
    x1 = CLAMP(x1, 1, map->width  - 2);
    x2 = CLAMP(x2, 1, map->width  - 2);
    y1 = CLAMP(y1, 1, map->height - 2);
    y2 = CLAMP(y2, 1, map->height - 2);

    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            cell_t *a = &map->item[y - 1][x - 1];
            cell_t *b = &map->item[y - 1][x  ];
            cell_t *c = &map->item[y - 1][x + 1];
            cell_t *d = &map->item[y  ][x - 1];
            cell_t *e = &map->item[y  ][x  ];
            cell_t *f = &map->item[y  ][x + 1];
            cell_t *g = &map->item[y + 1][x - 1];
            cell_t *h = &map->item[y + 1][x  ];
            cell_t *i = &map->item[y + 1][x + 1];
            e->lit = (
                         a->lit * 1 + b->lit * 2 + c->lit * 1 +
                         d->lit * 2 + e->lit * 3 + f->lit * 2 +
                         g->lit * 1 + h->lit * 2 + i->lit * 1) / 15;
        }
    }
}

/*
** Name: map_smooth_slopes
** Desc:
*/
void map_smooth_slopes(map_t *map, int x1, int y1, int x2, int y2)
{
    int x, y;
    x1 = CLAMP(x1, 1, map->width  - 2);
    x2 = CLAMP(x2, 1, map->width  - 2);
    y1 = CLAMP(y1, 1, map->height - 2);
    y2 = CLAMP(y2, 1, map->height - 2);

    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            cell_t *a = &map->item[y - 1][x - 1];
            cell_t *b = &map->item[y - 1][x  ];
            cell_t *c = &map->item[y - 1][x + 1];
            cell_t *d = &map->item[y  ][x - 1];
            cell_t *e = &map->item[y  ][x  ];
            cell_t *f = &map->item[y  ][x + 1];
            cell_t *g = &map->item[y + 1][x - 1];
            cell_t *h = &map->item[y + 1][x  ];
            cell_t *i = &map->item[y + 1][x + 1];
            e->bos = (
                         a->bos     + b->bos * 2 + c->bos     +
                         d->bos * 2 + e->bos * 3 + f->bos * 2 +
                         g->bos     + h->bos * 2 + i->bos) / 15;
            e->tos = (
                         a->tos     + b->tos * 2 + c->tos     +
                         d->tos * 2 + e->tos * 3 + f->tos * 2 +
                         g->tos     + h->tos * 2 + i->tos) / 15;
        }
    }
}

/******************************************************************************/

/*
** Name: cells_calc_lights
** Desc: Renders all the world lights. Lights are set using a switch
**       in each cell.
** Note: To prevent light escaping the world, the edges of the world are not
**       included for light checks.
*/
void map_calc_lights(map_t *map)
{
    int x, y, z;
    cell_t *cell;
    map_ambient_lit(map, i2f(0));

    for (y = 0; y < map->height; y++) {
        for (x = 0; x < map->width; x++) {
            cell = &map->item[y][x];

            if (cell->swi & CELL_SWI_LIGHT) {
                if (MAP_IS_EDGE(x, y)) {
                    cell->lit = CELL_LIGHT_SKY;
                } else {
                    map_draw_light(map, i2f(x) + FIXHALF, i2f(y) + FIXHALF);
                }
            }
        }
    }

    for (y = 1; y < map->height - 1; y++) {
        for (x = 1; x < map->width - 1; x++) {
            cell = &map->item[y][x];

            if (
                CELL_IS_OUTSIDE(&map->item[y  ][x  ]) &&
                CELL_IS_OUTSIDE(&map->item[y - 1][x  ]) &&
                CELL_IS_OUTSIDE(&map->item[y - 1][x - 1]) &&
                CELL_IS_OUTSIDE(&map->item[y  ][x - 1])) {
                z = (map->item[y][x].bots[0] - map->item[y - 1][x - 1].bots[0]) * 10;
                z = ABS(z);
                cell->lit = CELL_LIGHT_SKY - CLAMP(z, 0, CELL_LIGHT_SKY);
            }
        }
    }

    map_smooth_lit(map, MAP_RECT);
}

/******************************************************************************/

/*
** Name: map_init_slopes
** Desc: To increase rendering speed, slope values are precalculated and
**       cached in cells. This data is derived from the CUBE style terrain
**       offsets. This system uses more memory, but doesn't require code
**       to check for slopes.
*/
void map_init_slopes(map_t *map)
{
    int i, x, y;

    for (y = 0; y < map->height; y++) {
        for (x = 0; x < map->width; x++) {
            cell_t *c = &map->item[y][x];
            c->eos = 0;
            c->hos = 0;

            if (MAP_IS_EDGE(x, y)) {
                c->swi &= ~CELL_SWI_TERRAIN;
                c->swi &= ~CELL_SWI_CAVE;
                c->swi &= ~CELL_SWI_WARPED;
                c->wwi  = 0;
                c->bot  = 0;
                c->top  = 0;

                for (i = 4; i--;) {
                    c->bots[i] = c->bot;
                    c->tops[i] = c->top;
                }
            } else {
                for (i = 4; i--;) {
                    c->bots[i] = c->bot;
                    c->tops[i] = c->top;
                }

                if (c->swi & CELL_SWI_TERRAIN) {
                    c->bots[0] += map->item[y  ][x  ].bos << 5;
                    c->bots[1] += map->item[y  ][x + 1].bos << 5;
                    c->bots[2] += map->item[y - 1][x + 1].bos << 5;
                    c->bots[3] += map->item[y - 1][x  ].bos << 5;
                }

                if (c->swi & CELL_SWI_CAVE) {
                    c->tops[0] += map->item[y  ][x  ].tos << 5;
                    c->tops[1] += map->item[y  ][x + 1].tos << 5;
                    c->tops[2] += map->item[y - 1][x + 1].tos << 5;
                    c->tops[3] += map->item[y - 1][x  ].tos << 5;
                }
            }

            for (i = 4; i--;) {
                //if (c->bots[i] > c->tops[i]) c->bots[i] = c->tops[i];
            }
        }
    }

    for (y = 0; y < map->height; y++) {
        for (x = 0; x < map->width; x++) {
            cell_t *c = &map->item[y][x];
            int botmax = c->bots[0], botmin = c->bots[0];
            int topmax = c->tops[0], topmin = c->tops[0];

            for (i = 1; i < 4; i++) {
                if (c->bots[i] > botmax) {
                    botmax = c->bots[i];
                }

                if (c->bots[i] < botmin) {
                    botmin = c->bots[i];
                }

                if (c->tops[i] > topmax) {
                    topmax = c->tops[i];
                }

                if (c->tops[i] < topmin) {
                    topmin = c->tops[i];
                }
            }

            if (topmax > botmin) {
                c->swi &= ~CELL_SWI_SOLID;
            } else {
                c->swi |= CELL_SWI_SOLID;
            }

            if (!MAP_IS_EDGE(x, y)) {
                if (
                    map->item[y  ][x  ].wwi ||
                    map->item[y  ][x + 1].wwi ||
                    map->item[y + 1][x + 1].wwi ||
                    map->item[y + 1][x  ].wwi) {
                    c->swi |= CELL_SWI_WARPED;
                } else {
                    c->swi &= ~CELL_SWI_WARPED;
                }

                if (CELL_IS_OVERPASS(c)) {
                    int m = map->item[y  ][x - 1].bot + map->item[y  ][x - 1].eos;
                    m = MAX(map->item[y  ][x - 1].bot + map->item[y  ][x - 1].eos, m);
                    m = MAX(map->item[y  ][x + 1].bot + map->item[y  ][x + 1].eos, m);
                    m = MAX(map->item[y - 1][x  ].bot + map->item[y - 1][x  ].eos, m);
                    m = MAX(map->item[y + 1][x  ].bot + map->item[y + 1][x  ].eos, m);
                    c->eos = m - map->item[y][x].bot;
                    c->hos = c->eos - 64;
                }
            }
        }
    }
}

/*
** Name: map_precalc_data
** Desc:
*/
void map_precalc_data(map_t *map, int lod)
{
    map_init_slopes(map);
    map_init_lod(map, lod);
#ifndef __GBA__
    map_calc_lights(map);
#endif
}

/*
** Name: map_find_ent
** Desc:
*/
cell_t *map_find_ent(map_t *map, int ent, int *xp, int *yp)
{
    int x, y;

    for (y = map->height; y--;) {
        for (x = map->width; x--;) {
            if (map->item[y][x].ent == ent) {
                if (xp) {
                    *xp = x;
                }

                if (yp) {
                    *yp = y;
                }

                return &map->item[y][x];
            }
        }
    }

    return NULL;
}

/*
** Name: map_line_of_sight
** Desc: Returns TRUE if two points have a line of sight.
*/
int map_line_of_sight(map_t *map, int x1, int y1, int x2, int y2)
{
    if (
        x1 >= 0 && x1 < i2f(map->width) &&
        x2 >= 0 && x2 < i2f(map->width) &&
        y1 >= 0 && y1 < i2f(map->height) &&
        y2 >= 0 && y2 < i2f(map->height)) {
        x2 -= x1;
        y2 -= y1;
        {
            int x = ABS(x2), y = ABS(y2), i = f2i(MAX(x, y));
            x2 = r2i(x2 * reciprocal[i]);
            y2 = r2i(y2 * reciprocal[i]);

            for (; i--; x1 += x2, y1 += y2) {
                if (CELL_IS_SOLID(&map->item[f2i(y1)][f2i(x1)])) {
                    return FALSE;
                }
            }
        }
    }

    return TRUE;
}

