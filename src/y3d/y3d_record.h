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

#ifndef __Y3D_RECORD_H__
#define __Y3D_RECORD_H__

/******************************************************************************/

#include "y3d_keyboard.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

typedef struct record_t {
    FILE *file;
    int isplaying;
} record_t;

/******************************************************************************/

extern void record_rewind(record_t *record);
extern void record_write(record_t *record, keyboard_t *keyboard);
extern void record_read(record_t *record, keyboard_t *keyboard);
extern void record_start(record_t *record, char *filename);
extern void record_play(record_t *record, char *filename);
extern void record_play_or_record(record_t *record, char *filename);
extern void record_stop(record_t *record);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif
