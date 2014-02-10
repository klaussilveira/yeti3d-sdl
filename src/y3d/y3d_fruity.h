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

#ifndef __Y3D_FRUITY_H__
#define __Y3D_FRUITY_H__

/******************************************************************************/

#include "y3d_ansic.h"
#include "y3d_viewport.h"
#include "y3d_matrix.h"
#include "y3d_vertex.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

typedef struct framebuffer8_t {
    u8 pixels[240][320];
} framebuffer8_t;

typedef struct fruity_t {
    framebuffer8_t framebuffer;
    vertex_pool_t vpool;
    matrix_t m;
} fruity_t;

/******************************************************************************/

extern void fruity_draw(
    fruity_t *fruity,
    s16 *model,
    int pitch, int roll, int turn,
    int x, int y, int z);

extern void fruity_poly(polygon_t p, int n, u8 *dst, int pen);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif


