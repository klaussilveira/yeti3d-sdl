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

#ifndef __Y3D_TEXTURE_H__
#define __Y3D_TEXTURE_H__

#include "y3d_ansic.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define YETI_TEXGEN_LEFT  0
#define YETI_TEXGEN_RIGHT 1
#define YETI_TEXGEN_FRONT 2
#define YETI_TEXGEN_BACK  3
#define YETI_TEXGEN_TOP   4
#define YETI_TEXGEN_BOT   5

/******************************************************************************/

#define TEXTURE_BOT      5
#define TEXTURE_TOP      35
#define TEXTURE_WALL     11
#define TEXTURE_WATER    32
#define TEXTURE_LAVA     33
#define TEXTURE_SKY      35
#define TEXTURE_LIGHT1   96
#define TEXTURE_LIGHT2   109

/******************************************************************************/

#define TEXTURE_IS_SKY(A) ((A) == TEXTURE_SKY)

#define TEXTURE_IS_BRIGHT(A) (\
  (A) == TEXTURE_SKY     ||\
  (A) == TEXTURE_WATER   ||\
  (A) == TEXTURE_LAVA    ||\
  (A) == TEXTURE_LIGHT1  ||\
  (A) == TEXTURE_LIGHT2)

#define TEXTURE_IS_LIQUID(A) (\
  (A) == TEXTURE_WATER || \
  (A) == TEXTURE_LAVA )

// The maxium number of textures allowed.
#define TEXTURE_MAX     128
// The default texture size.
#define TEXTURE_WIDTH   64
#define TEXTURE_HEIGHT  64

/******************************************************************************/

typedef u8 texture_t[TEXTURE_HEIGHT][TEXTURE_WIDTH];

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif

