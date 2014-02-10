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

#ifndef __Y3D_FIXED_H__
#define __Y3D_FIXED_H__

#include "y3d_ansic.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

typedef int fixed; // Unsure if I should include this.

#define FIXONE  0x100
#define FIXHALF 0x80
#define FIXCEIL 0xFF

#define f2i(A) ((A)>>8)
#define i2f(A) ((A)<<8)
#define r2i(A) ((A)>>16)
#define i2fdiv2(A) ((A)<<7)
#define f2fl(A) ((A)/256.0)
#define fl2f(A) ((A)*256.0)
#define fixtrunc(A) i2f(f2i(A))
#define fixfrac(A) ((A)&FIXCEIL)
#define FIXLERP(A,B,C) ((A)+fixmul((B)-(A),(C)))
#define FIXSQR(A) fixmul(A,A)
#define fixup(A) f2i((A)+FIXCEIL)
#define fixinv(A) fixdiv(FIXUNIT,(A))
#define intdiv(A,B) ((A)/(B))
#define intmul(A,B) ((A)*(B))
#define muldiv(A,B,C) intdiv(intmul(A,B),(C))
#define fixmul(A,B) f2i(intmul(A,B))
#define fixmul2(A,B) intmul((A)>>4,(B)>>4)
#define fixdiv(A,B) intdiv(i2f((A)),(B))
#define fixdiv64(A,B) fixdiv((s64)(A),B)
#define FIXDIVXX(A,B) ((u32)(((s32)(A)>>23)+1)<2?fixdiv(A,B):fixdiv64(A,B))
#define fixsin(A) sintable[((A)>>5)&2047]
#define fixcos(A) fixsin((A)+DEG_90)
#define FIXDOT(A,B) f2i((A)[0]*(B)[0]+(A)[1]*(B)[1]+(A)[2]*(B)[2])

#define YETI_RECIPROCAL_MIN 0
#define YETI_RECIPROCAL_MAX 16384
#define YETI_SINTABLE_MAX 2048

/******************************************************************************/

#define DEG_1   i2f(1)
#define DEG_45  i2f(32)
#define DEG_90  i2f(64)
#define DEG_180 i2f(128)
#define DEG_270 i2f(192)
#define DEG_360 i2f(256)

/******************************************************************************/

extern u16 fixangle(int x, int y);

/******************************************************************************/

extern int reciprocal[YETI_RECIPROCAL_MAX];
extern int sintable[YETI_SINTABLE_MAX];

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif

