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

#include "y3d_vertex.h"
#include "y3d_fixed.h"

/*
** Name: polycliper
** Desc: Clips a polygon to pre-calculated plane distances. Clip polygons
**       are arrays of cached vertex pointers. This decreases data movement
**       while allowing vertices to be reused by neighbouring polygons.
*/
int polycliper(
    vertex_t **dst,
    vertex_t **src,
    int n,
    vertex_pool_t *vpool)
{
    vertex_t **tmp = dst;

    if (n > 2) {
        vertex_t *pre;

        for (pre = src[n - 1]; n--; pre = *src++) {
            if (pre->d >= 0) {
                *dst++ = pre;
            }

            if ((pre->d ^ src[0]->d) < 0) {
#ifdef YETI_LOW_QUALITY_CLIPPING
                int r = f2i((r = pre->d - src[0]->d) < 0 ? reciprocal[-r] * -pre->d : reciprocal[r] * pre->d);
                dst[0]    = VERTEX_POOL_GET(vpool);
                dst[0]->x = FIXLERP(pre->x, src[0]->x, r);
                dst[0]->y = FIXLERP(pre->y, src[0]->y, r);
                dst[0]->z = FIXLERP(pre->z, src[0]->z, r);
                dst[0]->u = FIXLERP(pre->u, src[0]->u, r);
                dst[0]->v = FIXLERP(pre->v, src[0]->v, r);
                dst[0]->l = FIXLERP(pre->l, src[0]->l, r);
#else
#ifdef YETI_MEDIUM_QUALITY_CLIPPING
                int r = fixdiv(pre->d, pre->d - src[0]->d);
                dst[0]    = VERTEX_POOL_GET(vpool);
                dst[0]->x = FIXLERP(pre->x, src[0]->x, r);
                dst[0]->y = FIXLERP(pre->y, src[0]->y, r);
                dst[0]->z = FIXLERP(pre->z, src[0]->z, r);
                dst[0]->u = FIXLERP(pre->u, src[0]->u, r);
                dst[0]->v = FIXLERP(pre->v, src[0]->v, r);
                dst[0]->l = FIXLERP(pre->l, src[0]->l, r);
#else
                int r = pre->d - src[0]->d;
                dst[0]    = VERTEX_POOL_GET(vpool);
                dst[0]->x = pre->x + intdiv(intmul(src[0]->x - pre->x, pre->d), r);
                dst[0]->y = pre->y + intdiv(intmul(src[0]->y - pre->y, pre->d), r);
                dst[0]->z = pre->z + intdiv(intmul(src[0]->z - pre->z, pre->d), r);
                dst[0]->u = pre->u + intdiv(intmul(src[0]->u - pre->u, pre->d), r);
                dst[0]->v = pre->v + intdiv(intmul(src[0]->v - pre->v, pre->d), r);
                dst[0]->l = pre->l + intdiv(intmul(src[0]->l - pre->l, pre->d), r);
#endif
#endif
                VERTEX_PROJECT_SLOW(dst[0]);
                dst++;
            }
        }
    }

    return dst - tmp;
}
/******************************************************************************/

