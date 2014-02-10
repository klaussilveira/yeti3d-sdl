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

#ifndef __Y3D_SURFACE_H__
#define __Y3D_SURFACE_H__

#include "y3d_texture.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define SURFACE_MAX           TEXTURE_MAX
#define SURFACE_DEFAULT_SIZE  4

/******************************************************************************/

typedef struct surface_t {
    u8  xsize: 4; // Texture x scale.
    u8  ysize: 4; // Texture y scale.
    u8  mode;     // Rendering mode.
    s16 xpan;     // 8:8 texture u offset.
    s16 ypan;     // 8:8 texture v offset.
} surface_t;

typedef STATICARRAY(surface_t, SURFACE_MAX) surface_array_t;

/******************************************************************************/

extern void surface_init(surface_t *surface);
extern void surface_array_init(surface_array_t *surfaces);

/******************************************************************************/

#ifdef __cplusplus
};
#endif

/******************************************************************************/

#endif