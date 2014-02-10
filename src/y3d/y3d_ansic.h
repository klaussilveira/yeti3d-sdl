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

#ifndef __Y3D_ANSIC_H__
#define __Y3D_ANSIC_H__

/******************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

/******************************************************************************/

typedef volatile s8			vs8;
typedef volatile s16		vs16;
typedef volatile s32		vs32;
typedef volatile s64		vs64;

typedef volatile u8     vu8;
typedef volatile u16		vu16;
typedef volatile u32		vu32;
//typedef volatile u64		vu64;

/******************************************************************************/

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

/******************************************************************************/

#define ABS(A)   ((A)<(0)?-(A):(A))
#define MIN(A,B) ((A)<(B)? (A):(B))
#define MAX(A,B) ((A)>(B)? (A):(B))
#define SQR(A)   ((A)*(A))

#define SWAP(T,A,B) {T _temp_=(A);(A)=(B);(B)=_temp_;}

#define ISNEG(A) ((A)<0)

/******************************************************************************/

#define CLAMP(A,B,C) MIN(MAX(A,B),C)

#define CLEARMEM(A) memset(A,0,sizeof(*(A)))

#define BITSET(flag_var, bit_msk) (flag_var |= bit_msk)
#define BITCLR(flag_var, bit_msk) (flag_var &= ~bit_msk)
#define BITTST(flag_var, bit_msk) (flag_var & bit_msk)
#define BITSHF(A,B) ((B)<0?(A)<<-(B):(A)>>(B))

#define STATICARRAY(TYPE,SIZE) struct {int length; TYPE item[SIZE];}
#define STATICARRAY2D(TYPE,W,H) struct {int width, height; TYPE item[H][W];}
#define STATICARRAY_MAX(A) (sizeof((A)->item)/sizeof((A)->item[0]))

#define DYNAMICARRAY(TYPE) struct {int length; TYPE* item;}

#ifdef __GBA__
#include "mushroom/include/mushroom/msh_dma.h"
#define memcpy16(A,B,C) DMACPY16(3,A,B,C)
#else
#define memcpy16(A,B,C) memcpy(A,B,(C)<<1)
#define memset16(A,B,C) memset(A,B,(C)<<1)
#endif

/******************************************************************************/

#define INTMUL2(A) ((A)<<1)
#define INTMUL3(A) (INTMUL2(A)+(A))
#define INTMUL4(A) ((A)<<2)
#define INTMUL6(A) (INTMUL4(A)+INTMUL2(A))

/******************************************************************************/

typedef char string_t[256];

typedef struct rgb_t {
    u8 r, g, b;
} rgb_t;

typedef struct rgba_t {
    u8 r, g, b, a;
} rgba_t;

typedef struct point_t {
    s32 x, y;
} point_t;

typedef struct rect_t {
    s32 x1, y1, x2, y2;
} rect_t;

typedef struct vec2_t {
    s32 x, y;
} vec2_t;

typedef struct vec3_t {
    s32 x, y, z;
} vec3_t;

/******************************************************************************/

extern char *ansic_stpcpy(char *dst, char *src);
extern void ansic_itos(int i, char *s);
extern int ansic_friction(int a, int b);

extern int isqrt(int value);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif
