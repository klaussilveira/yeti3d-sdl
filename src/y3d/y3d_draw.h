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

#ifndef __Y3D_DRAW_H__
#define __Y3D_DRAW_H__

/******************************************************************************/

#include "y3d_yeti.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define DRAW_MODE_COLORKEY        (0)
#define DRAW_MODE_COLORKEY_BLEND  (1)
#define DRAW_MODE_BLEND           (2)
#define DRAW_MODE_BLEND_DITHER    (3)
#define DRAW_MODE_COPY            (4)
#define DRAW_MODE_COPY_DITHER     (5)
#define DRAW_MODE_COPY_HALFBRIGHT (6)

/******************************************************************************/

#define XDITHER(I) ((((int)&fb[I])>>1)&1)

/******************************************************************************/

#define AFFINE_CASE(I) case(I):AFFINE(0);fb++;

#define AFFINE_LOOP8()\
for(;i>=8;i-=8,fb+=8){\
AFFINE( 0)AFFINE( 1)AFFINE( 2)AFFINE( 3)AFFINE( 4)AFFINE( 5)AFFINE( 6)AFFINE( 7)\
}switch(i){\
AFFINE_CASE( 8)AFFINE_CASE( 7)AFFINE_CASE( 6)AFFINE_CASE( 5)\
AFFINE_CASE( 4)AFFINE_CASE( 3)AFFINE_CASE( 2)AFFINE_CASE( 1)\
}

#define AFFINE_LOOP16()\
for(;i>=16;i-=16,fb+=16){\
AFFINE( 0)AFFINE( 1)AFFINE( 2)AFFINE( 3)AFFINE( 4)AFFINE( 5)AFFINE( 6)AFFINE( 7)\
AFFINE( 8)AFFINE( 9)AFFINE(10)AFFINE(11)AFFINE(12)AFFINE(13)AFFINE(14)AFFINE(15)\
}switch(i){\
AFFINE_CASE(16)AFFINE_CASE(15)AFFINE_CASE(14)AFFINE_CASE(13)\
AFFINE_CASE(12)AFFINE_CASE(11)AFFINE_CASE(10)AFFINE_CASE( 9)\
AFFINE_CASE( 8)AFFINE_CASE( 7)AFFINE_CASE( 6)AFFINE_CASE( 5)\
AFFINE_CASE( 4)AFFINE_CASE( 3)AFFINE_CASE( 2)AFFINE_CASE( 1)\
}

#define AFFINE_LOOP AFFINE_LOOP8

/******************************************************************************/

#define Y1_INIT  999999
#define Y2_INIT -999999

/******************************************************************************/

typedef struct dither_kernal_t {
    int u[2], v[2];
} dither_kernal_t;

/******************************************************************************/

extern void fill_rect(pixel_buffer_t *dst, int x1, int y1, int x2, int y2, int pixel);
extern void draw_char(pixel_buffer_t *fb, int x, int y, int c, int xs, int ys, int mode);
extern void draw_string(pixel_buffer_t *fb, int x, int y, char *s, int xs, int ys, int mode, int os);

extern void draw_poly_line_nc(
    pixel_buffer_t *dst,
    polyclip_t p, int n,
    int color);

extern void tmap_16_sky(
    polygon_t p, int n,
    u16 *dst,
    u16 *src1, int xos1,
    u16 *src2, int xos2);

extern void tmap_16(polygon_t p, int n, u16 *dst, u16 *src, int mode);
extern void tmap_16_cs(polygon_t p, int n, u16 *dst, u16 *src, int mode);
extern void tmap_8_horz(polygon_t p, int n, u16 *dst, u8 *src, lua_t lua, int mode);
extern void tmap_8_horz_pc(polygon_t p, int n, u16 *dst, u8 *src, lua_t lua, int mode);

/******************************************************************************/

#ifdef __cplusplus
};
#endif

/******************************************************************************/

#endif

