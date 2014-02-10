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

#ifndef __Y3D_SOUND_H__
#define __Y3D_SOUND_H__

/******************************************************************************/

#include "y3d_yeti.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define SOUND_EXPLOSION     1
#define SOUND_SHOOT         2
#define SOUND_TICK          3
#define SOUND_ROCKETLAUNCH  4
#define SOUND_ITEMPICK      5
#define SOUND_ITEMBACK      6
#define SOUND_JUMP          7
#define SOUND_SPLASH        8
#define SOUND_FOOTSTEPS     9

extern void sound_play(int id);
extern void sound_loop(int id);
extern void sound_stop(int id);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif

