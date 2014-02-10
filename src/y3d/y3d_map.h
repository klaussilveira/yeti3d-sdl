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

#ifndef __Y3D_MAP_H__
#define __Y3D_MAP_H__

/******************************************************************************/

#include "y3d_cell.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define MAP_WIDTH 64
#define MAP_HEIGHT 64
#define MAP_RECT 0, 0, MAP_WIDTH, MAP_HEIGHT
#define MAP_IS_EDGE(X,Y) ((X)==0||(Y)==0||((X)==MAP_WIDTH-1)||((Y)==MAP_HEIGHT-1))

typedef STATICARRAY2D(cell_t, MAP_WIDTH, MAP_HEIGHT) map_t;

//typedef cell_t map_t[MAP_HEIGHT][MAP_WIDTH];

typedef struct rom_map_t {
    char name[32];
    char auth[32];
    char desc[64];
    rom_cell_t cells[MAP_HEIGHT][MAP_WIDTH];
} rom_map_t;

/******************************************************************************/

extern void map_init(map_t *map, int lod);
extern void map_smooth_slopes(map_t *map, int x1, int y1, int x2, int y2);
extern void map_precalc_data(map_t *map, int lod);

extern cell_t *map_find_ent(map_t *map, int ent, int *xp, int *yp);
extern int map_line_of_sight(map_t *map, int x1, int y1, int x2, int y2);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif
