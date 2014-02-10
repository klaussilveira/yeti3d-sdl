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

#ifndef __Y3D_ANIMATION_H__
#define __Y3D_ANIMATION_H__

/******************************************************************************/

#include "y3d_ansic.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define LOOP_ONCE     1
#define LOOP_FOREVER  100

typedef struct keyframe_t {
    s16 x;
    s16 y;
    s16 z;
    s16 r;
    s16 t;
    s16 p;
} keyframe_t;

typedef struct animation_t {
    s8  mode;
    u8  nloops;
    u16 pos;
    u16 start;
    u16 length;
    u16 end;
    u16 speed;
} animation_t;

/******************************************************************************/

extern void animation_init(animation_t *pAnimation, int mode, int start, int length, int speed, int loops);
extern void animation_tick(animation_t *pAnimation);
extern void animation_mode(animation_t *pAnimation, int mode, int speed, int loops);
extern void animation_get_frames(animation_t *animation, int *f1, int *f2);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif

