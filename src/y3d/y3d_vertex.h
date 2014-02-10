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

#ifndef __Y3D_VERTEX_H__
#define __Y3D_VERTEX_H__

#include "y3d_ansic.h"
#include "y3d_viewport.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define VERTEX_POOL_MAX 1024
#define VERTEX_POOL_MASK (VERTEX_POOL_MAX - 1)

/******************************************************************************/

// Type definition of a polygon vertex
typedef struct vertex_t {
    int x;  // 24:8 Fixed Point X Position.
    int y;  // 24:8 Fixed Point Y Position.
    int z;  // 24:8 Fixed Point Z Position.
    int l;  // 24:8 Fixed point lighting. Ranges from 0 to 63.
    int d;  // Distance to plane value.
    int i;  // Vertex ID. Used to lookup a vertex in a cell vcache.
    int u;  // 24:8 texture 'u' cooridinate.
    int v;  // 24:8 texture 'v' cooridinate.
    int sx; // 24:8 X projected into screen space.
    int sy; // 24:8 Y projected into screen space.
} vertex_t;

// Returns True if a vertex is outside the view pyramid.
#define VERTEX_IS_OUTSIDE(A) (((A)->x+(A)->z)<0||((A)->z-(A)->x)<0||((A)->y+(A)->z)<0||((A)->z-(A)->y)<0)

#define POLYGON_MAX 10
typedef vertex_t *polyclip_t[POLYGON_MAX];

// Returns True if a projected triangle is visible.
#define POLYCLIP_IS_VISIBLE(A) (\
  (((A)[2]->sx-(A)[1]->sx)>>3)*(((A)[0]->sy-(A)[1]->sy)>>3) - \
  (((A)[2]->sy-(A)[1]->sy)>>3)*(((A)[0]->sx-(A)[1]->sx)>>3) > 0)

typedef STATICARRAY(vertex_t, VERTEX_POOL_MAX) vertex_pool_t;

#define VERTEX_POOL_GET(A) (&(A)->item[(A)->length++ & VERTEX_POOL_MASK])

/******************************************************************************/

#define VERTEX_PROJECT_FAST(V)\
if((V)->z>YETI_RECIPROCAL_MIN&&(V)->z<YETI_RECIPROCAL_MAX){\
int z=reciprocal[(V)->z];\
(V)->sx=i2f(YETI_VIEWPORT_XCENTER)+((YETI_VIEWPORT_WIDTH *(V)->x*z)>>9);\
(V)->sy=i2f(YETI_VIEWPORT_YCENTER)-((YETI_VIEWPORT_HEIGHT*(V)->y*z)>>9);\
}else{(V)->sx=(V)->sy=0;}

#define VERTEX_PROJECT_SLOW(V)\
if((V)->z>0){\
(V)->sx=i2f(YETI_VIEWPORT_XCENTER)+(i2fdiv2(YETI_VIEWPORT_WIDTH )*(V)->x/(V)->z);\
(V)->sy=i2f(YETI_VIEWPORT_YCENTER)-(i2fdiv2(YETI_VIEWPORT_HEIGHT)*(V)->y/(V)->z);\
}else{(V)->sx=(V)->sy=0;}

/******************************************************************************/

// Transformed/lit vertices
typedef struct tlvertex_t {
    int x;
    int y;
    int z;
    int l;
    int v;
    int u;
} tlvertex_t;

typedef tlvertex_t polygon_t[POLYGON_MAX];

/******************************************************************************/

extern int polycliper(vertex_t **dst, vertex_t **src, int n, vertex_pool_t *vpool);

/******************************************************************************/

#ifdef __cplusplus
};
#endif

/******************************************************************************/

#endif

