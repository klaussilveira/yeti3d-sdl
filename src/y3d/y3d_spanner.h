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

#ifndef __Y3D_SPANNER_H__
#define __Y3D_SPANNER_H__

/******************************************************************************/

#include "y3d_ansic.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define SPANNER_MAX_HEIGHT 480

/******************************************************************************/

typedef struct span_t {
    int x1, x2;
} span_t;

typedef struct spanarray_t {
    int nspans;
    span_t  spans[100];
} spanarray_t;

typedef struct spanner_t {
    int   width;
    int   height;
    int   ngaps;
    spanarray_t  lines[SPANNER_MAX_HEIGHT];
} spanner_t;

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif
