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

#ifndef __GAME_H__
#define __GAME_H__

/******************************************************************************/

#include "../y3d/y3d_yeti.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define GAME_EPISODE_MAX      27

/******************************************************************************/

#define GAME_MODE_PUBLISHER   0
#define GAME_MODE_YETI3D      1
#define GAME_MODE_TITLE       2
#define GAME_MODE_MENU        3
#define GAME_MODE_PLAY        4
#define GAME_MODE_CREDITS     5
#define GAME_MODE_OPTIONS     6
#define GAME_MODE_SKILL       7
#define GAME_MODE_EPISODE     8
#define GAME_MODE_PAUSED      9
#define GAME_MODE_LOADGAME    10
#define GAME_MODE_QUITGAME    11
#define GAME_MODE_START_LEVEL 12
#define GAME_MODE_ENDGAME     13
#define GAME_MODE_ENDLEVEL    14

/******************************************************************************/

extern void game_init(yeti_t *yeti);
extern void game_draw(yeti_t *yeti);
extern void game_tick(yeti_t *yeti);
extern void game_loop(yeti_t *yeti);
extern void game_goto(game_t *game, int mode);

extern void game_load_map(yeti_t *yeti, rom_map_t *map);
extern void game_spawn_entities(yeti_t *yeti);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif

