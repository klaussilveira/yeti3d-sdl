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

#ifndef __Y3D_CELL_H__
#define __Y3D_CELL_H__

/******************************************************************************/

#include "y3d_entity.h"
#include "y3d_vertex.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

// These cell flags are stored in both the engine and yeti maps.
#define CELL_SWI_LIGHT      1
#define CELL_SWI_SOLID      2
#define CELL_SWI_TERRAIN    4
#define CELL_SWI_WARPED     8
#define CELL_SWI_LUA_0      0
#define CELL_SWI_LUA_1      16
#define CELL_SWI_LUA_2      32
#define CELL_SWI_LUA_3      48
#define CELL_SWI_LUA_MASK   48
#define CELL_SWI_CAVE       64
#define CELL_SWI_SELECTED   128

// These switchs are for the new extra switchs in cell.ext.swi
#define CELL_SW2_LIGHT0       0
#define CELL_SW2_LIGHT1       1
#define CELL_SW2_LIGHT2       2
#define CELL_SW2_LIGHT3       3
#define CELL_SW2_LIGHT_MASK   3
#define CELL_SW2_OVERPASS     4
#define CELL_SW2_HALFBRIGHT   128

/******************************************************************************/

#define CELL_IS_SOLID(A)      ((A)->swi & CELL_SWI_SOLID)
#define CELL_IS_WARPED(A)     ((A)->swi & CELL_SWI_WARPED)
#define CELL_IS_SELECTED(A)   ((A)->swi & CELL_SWI_SELECTED)
#define CELL_IS_TERRAIN(A)    ((A)->swi & CELL_SWI_TERRAIN)
#define CELL_IS_CAVE(A)       ((A)->swi & CELL_SWI_CAVE)
#define CELL_IS_OVERPASS(A)   ((A)->sw2 & CELL_SW2_OVERPASS)
#define CELL_IS_OUTSIDE(A)    ((A)->ttx == TEXTURE_SKY && !CELL_IS_SOLID(A))
#define CELL_IS_OPAQUE(A)     (CELL_IS_SOLID(A) && !CELL_IS_WARPED(A))

/******************************************************************************/

#define CELL_LUA(A) (((A)->swi & CELL_SWI_LUA_MASK) >> 4)
#define CELL_MARK(A) ((A)->mark = yeti->mark)
#define CELL_IS_MARKED(A) ((A)->mark == yeti->mark)
#define CELL_UNMARK(A) ((A)->mark = (yeti->mark - 1))
#define CELL_UNMARK_ALL() (yeti->mark++)

#define CELL_VCACHE_MAX 3
#define CELL_LIGHT_MIN i2f(2)
#define CELL_LIGHT_MAX i2f(63)
#define CELL_LIGHT_SKY i2f(32)

/******************************************************************************/

typedef struct rom_cell_t {
    u8  swi;  // Cell switchs (flags).
    u8  ent;  // Entity Id. Used for setting start locations for entities.
    s16 top;  // 8:8 fixed point cell top value.
    u8  wtb;  // Cell wall bot texture id.
    u8  sw2;  // Cell switch extra.
    s16 bot;  // 8:8 fixed point cell bot value.
    s16 lit;  // Cell light setting.
    u8  wtt;  // Cell wall top texture id.
    u8  ttx;  // Cell top texture id.
    u8  wwi;  // Wall warp index.
    u8  btx;  // Cell bot texture id.
    s8  tos;  // Top offset.
    s8  bos;  // Bottom offset.
} rom_cell_t;

typedef struct cell_t {
    u8  swi;  // Cell flags.
    u8  ent;  // Entity Id. Used for setting start locations for entities.
    s16 top;  // 8:8 fixed point cell top value.
    s16 bot;  // 8:8 fixed point cell bot value.
    s16 eos;  // Entity offset.
    s16 hos;  // Height offset.
    s8  tos;  // Top offset. Used for caves.
    s8  bos;  // Bottom offset. Used for terrains.
    s16 bots[4];
    s16 tops[4];
#ifndef __GBA__
    // Note: These variables are only used by the editor, so they can be undefined
    // for platforms that need a little more ram.
    s16 tsa;  // Top save.
    s16 bsa;  // Bot save.
#endif
    u8  wtb;  // Cell wall bot texture id.
    u8  sw2;
    s16 lit;  // Cell light setting.
    u8  wtt;  // Cell wall top texture id.
    u8  ttx;  // Cell top texture id.
    u8  btx;  // Cell bot texture id.
    u8  wwi;  // Wall warp index.

    s8  xlod;
    s8  ylod;
    u32 mark;
    entity_t *entities;

    u8 vcache_size;
    vertex_t *vcache[CELL_VCACHE_MAX];
} cell_t;

/******************************************************************************/

extern void cell_init(cell_t *cell, int issolid, int islight);
extern int cells_are_equal(cell_t *c1, cell_t *c2);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif
