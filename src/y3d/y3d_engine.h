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

#ifndef __Y3D_ENGINE_H__
#define __Y3D_ENGINE_H__

#include "y3d_yeti.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
typedef vec3_t quadrant_t[4];

typedef struct bucket_node_t {
    s16 next;
    s16 index;
} bucket_node_t;

/******************************************************************************/

extern void yeti_tick(yeti_t *yeti);
extern void yeti_draw(yeti_t *yeti);
extern void yeti_model_draw(yeti_t *yeti, entity_t *e);

extern int cell_bot(cell_t *cell, entity_t *e);
extern int cell_top(cell_t *cell, entity_t *e);

/******************************************************************************/

#ifdef __cplusplus
};
#endif

/******************************************************************************/

#endif
