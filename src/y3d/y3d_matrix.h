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

#ifndef __Y3D_MATRIX_H__
#define __Y3D_MATRIX_H__

#include "y3d_fixed.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

typedef fixed matrix_t[3][4];

/******************************************************************************/

extern void matrix_rotate_world(matrix_t m, int r, int p, int t);
extern void matrix_rotate_object(matrix_t m, int p, int r, int t, int x, int y, int z);
extern void matrix_multiply(matrix_t a, matrix_t b, matrix_t c);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif


