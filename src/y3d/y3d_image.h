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

#ifndef __Y3D_IMAGE_H__
#define __Y3D_IMAGE_H__

#include "y3d_ansic.h"
#include "y3d_viewport.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define IMAGE_HEADER (2)
#define IMAGE_WIDTH(A) ((A)[0])
#define IMAGE_HEIGHT(A) ((A)[1])
#define IMAGE_PIXEL(A,X,Y) ((A)[(Y)*IMAGE_WIDTH(A)+(X)+IMAGE_HEADER])
#define IMAGE_SCANLINE(A,Y) (&IMAGE_PIXEL(A,0,Y))
#define IMAGE_RECT(A) 0, 0, IMAGE_WIDTH(A), IMAGE_HEIGHT(A)
#define IMAGE(A) (A), IMAGE_RECT(A)

/******************************************************************************/

extern void image_draw(
    u16 *dst, int x1, int y1, int x2, int y2,
    u16 *src, int u1, int v1, int u2, int v2,
    int mode);

extern void image_draw_at(u16 *dst, u16 *src, int x, int y, int mode);

extern void image_draw_di(
    u16 *dst, int x1, int y1, int x2, int y2, int z,
    u16 *src, int u1, int v1, int u2, int v2, int mode);

/******************************************************************************/

#ifdef __cplusplus
};
#endif

/******************************************************************************/

#endif

