/*
** Name: Yeti3D
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

#ifndef __SCREENS_H__
#define __SCREENS_H__

#include "../y3d/y3d_yeti.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

extern void screen_publisher(yeti_t *yeti);
extern void screen_flames(yeti_t *yeti, int rising);
extern void screen_yeti3d(yeti_t *yeti);
extern void screen_title(yeti_t *yeti);
extern void screen_menu(yeti_t *yeti);
extern void screen_episode(yeti_t *yeti);
extern void screen_paused(yeti_t *yeti);
extern void screen_play(yeti_t *yeti);
extern void screen_credits(yeti_t *yeti);
extern void screen_view_options(yeti_t *yeti);
extern void screen_load_game(yeti_t *yeti);
extern void screen_quit_game(yeti_t *yeti);
extern void screen_skill(yeti_t *yeti);
extern void screen_end_level(yeti_t *yeti);

/******************************************************************************/

#ifdef __cplusplus
};
#endif

/******************************************************************************/

#endif

