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

#include "y3d_fruity.h"
#include "y3d_matrix.h"
#include "y3d_fixed.h"
#include "y3d_draw.h"

/*
** Name: fruity_draw
** Desc:
*/
void fruity_draw(
    fruity_t *fruity,
    s16 *model,
    int pitch, int roll, int turn,
    int x, int y, int z)
{
    int i = 0, j = 0, k = 0, n = 0;
    vec3_t verts[500];
    matrix_t om, wm;
    polyclip_t p;
    polygon_t polygon;
    model++;
    matrix_rotate_object(om, roll, pitch, turn, x, y, z);
    matrix_multiply(wm, om, fruity->m);

    for (n = *model++, i = 0; i < n; i++) {
        int x = *model++;
        int y = *model++;
        int z = *model++;
        verts[i].x = f2i(wm[0][0] * x + wm[0][1] * y + wm[0][2] * z) + wm[0][3];
        verts[i].y = f2i(wm[1][0] * x + wm[1][1] * y + wm[1][2] * z) + wm[1][3];
        verts[i].z = f2i(wm[2][0] * x + wm[2][1] * y + wm[2][2] * z) + wm[2][3];
    }

    for (i = *model++; i--;) {
        for (j = n = *model++; j--;) {
            p[j]    = VERTEX_POOL_GET(&fruity->vpool);
            p[j]->x = verts[*model].x;
            p[j]->y = verts[*model].y;
            p[j]->z = verts[*model].z;
            k = (z - p[j]->z) << 3; //((x - p[j]->x) + (z - p[j]->z) - (y - p[j]->y)) << 3;

            if (k < i2f(4)) {
                k = i2f(4);
            }

            if (k > i2f(31)) {
                k = i2f(31);
            }

            p[j]->l = k;
            model += 1;
            VERTEX_PROJECT_SLOW(p[j]);
        }

        if (POLYCLIP_IS_VISIBLE(p)) {
            for (k = n; k--;) {
                vertex_t *vertex = p[k];

                if (VERTEX_IS_OUTSIDE(vertex)) {
                    polyclip_t a;

                    for (k = n; k--; p[k]->d = p[k]->x + p[k]->z);

                    n = polycliper(a, p, n, &fruity->vpool); // Left

                    for (k = n; k--; a[k]->d = a[k]->z - a[k]->x);

                    n = polycliper(p, a, n, &fruity->vpool); // Right

                    for (k = n; k--; p[k]->d = p[k]->y + p[k]->z);

                    n = polycliper(a, p, n, &fruity->vpool); // Top

                    for (k = n; k--; a[k]->d = a[k]->z - a[k]->y);

                    n = polycliper(p, a, n, &fruity->vpool); // Bottom
                    break;
                }
            }

            if (n > 2) {
                for (j = n; j--;) {
                    polygon[j].x = p[j]->sx;
                    polygon[j].y = p[j]->sy;
                    polygon[j].l = p[j]->l;
                }

                fruity_poly(polygon, n, fruity->framebuffer.pixels[0], *model);
            }
        }

        model++;
    }
}

/*
** Name: fruity_poly
** Desc:
*/
void fruity_poly(polygon_t p, int n, u8 *dst, int pen)
{
    int lt_i = 0, lt_x = 0, lt_xx = 0, lt_l = 0, lt_ll = 0;
    int rt_i = 0, rt_x = 0, rt_xx = 0, rt_l = 0, rt_ll = 0;
    int x1 = 0, x2 = 0, i = 0;
    int lt_length = 0, rt_length = 0;
    int y1 = Y1_INIT, y2 = Y2_INIT, d[POLYGON_MAX];
    pen <<= 5;

    for (i = n; i--;) {
        d[i] = fixup(p[i].y);

        if (p[i].y < y1) {
            y1 = p[i].y;
            lt_i = rt_i = i;
        }

        if (p[i].y > y2) {
            y2 = p[i].y;
        }
    }

    if ((y1 = fixup(y1)) < YETI_VIEWPORT_Y1) {
        y1 = YETI_VIEWPORT_Y1;
    }

    if ((y2 = fixup(y2)) > YETI_VIEWPORT_Y2) {
        y2 = YETI_VIEWPORT_Y2;
    }

    for (dst += y1 * YETI_FRAMEBUFFER_WIDTH; y1 < y2; y1++, dst += YETI_FRAMEBUFFER_WIDTH) {
        if (--lt_length <= 0) {
            do {
                i = lt_i--;

                if (lt_i < 0) {
                    lt_i = n - 1;
                }
            } while ((lt_length = d[lt_i] - d[i]) <= 0);

            lt_xx = r2i((p[lt_i].x - (lt_x = p[i].x)) * reciprocal[lt_length]);
            lt_ll = r2i((p[lt_i].l - (lt_l = p[i].l)) * reciprocal[lt_length]);
        }

        if (--rt_length <= 0) {
            do {
                i = rt_i++;

                if (rt_i >= n) {
                    rt_i = 0;
                }
            } while ((rt_length = d[rt_i] - d[i]) <= 0);

            rt_xx = r2i((p[rt_i].x - (rt_x = p[i].x)) * reciprocal[rt_length]);
            rt_ll = r2i((p[rt_i].l - (rt_l = p[i].l)) * reciprocal[rt_length]);
        }

        if ((x1 = fixup(lt_x)) < YETI_VIEWPORT_X1) {
            x1 = YETI_VIEWPORT_X1;
        }

        if ((x2 = fixup(rt_x)) > YETI_VIEWPORT_X2) {
            x2 = YETI_VIEWPORT_X2;
        }

        if ((i = x2 - x1) > 0) {
            u8 *fb = &dst[x1];
            int l = lt_l, ll = r2i((rt_l - lt_l) * reciprocal[i]);
#define AFFINE(I) fb[I]=f2i(l)|pen;l+=ll;
            AFFINE_LOOP();
#undef  AFFINE
        }

        lt_x += lt_xx;
        lt_l += lt_ll;
        rt_x += rt_xx;
        rt_l += rt_ll;
    }
}
