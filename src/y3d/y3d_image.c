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

#include "y3d_image.h"
#include "y3d_fixed.h"
#include "y3d_draw.h"

/*
** Name: image_draw
** Desc: Draws a 16bit image to a framebuffer. The image is clipped to the
**       viewport. Multiple image rendering modes are avaliable.
*/
#ifndef __PATCH_IMAGE_DRAW__
void image_draw(
    u16 *dst, int x1, int y1, int x2, int y2,
    u16 *src, int u1, int v1, int u2, int v2,
    int mode)
{
    int w = x2 - x1;
    int h = y2 - y1;

    if (w > 0 && h > 0 && w < YETI_RECIPROCAL_MAX && h < YETI_RECIPROCAL_MAX) {
        int uu = f2i((u2 - u1) * reciprocal[w]);
        int vv = f2i((v2 - v1) * reciprocal[h]);
        u1 = i2f(u1);
        v1 = i2f(v1);

        if (x1 < YETI_VIEWPORT_X1) {
            u1 += (YETI_VIEWPORT_X1 - x1) * uu;
            x1 = YETI_VIEWPORT_X1;
        }

        if (y1 < YETI_VIEWPORT_Y1) {
            v1 += (YETI_VIEWPORT_Y1 - y1) * vv;
            y1 = YETI_VIEWPORT_Y1;
        }

        if (x2 > YETI_VIEWPORT_X2) {
            x2 = YETI_VIEWPORT_X2;
        }

        if (y2 > YETI_VIEWPORT_Y2) {
            y2 = YETI_VIEWPORT_Y2;
        }

        if ((x2 - x1) > 0 && (y2 - y1) > 0) {
            dst += y1 * YETI_FRAMEBUFFER_WIDTH + x1;

            switch (mode) {
            case DRAW_MODE_COLORKEY: {
                for (; y1 < y2; y1++, v1 += vv, dst += YETI_FRAMEBUFFER_WIDTH) {
                    u16 *tb = IMAGE_SCANLINE(src, f2i(v1));
                    u16 *fb = dst, c;
                    int i = x2 - x1, u = u1;
#define AFFINE(I) if((c=tb[f2i(u)])!=0)fb[I]=c;u+=uu;
                    AFFINE_LOOP();
#undef  AFFINE
                }

                break;
            }

            case DRAW_MODE_COLORKEY_BLEND: {
                for (; y1 < y2; y1++, v1 += vv, dst += YETI_FRAMEBUFFER_WIDTH) {
                    u16 *tb = IMAGE_SCANLINE(src, f2i(v1));
                    u16 *fb = dst, c;
                    int i = x2 - x1, u = u1;
#define AFFINE(I) if((c=tb[f2i(u)])!=0) fb[I]=RGB_BLEND(c,fb[I]);u+=uu;
                    //#define AFFINE(I) if((c=tb[f2i(u)])!=0 && (XDITHER(I)^(y1&1)))fb[I]=c;u+=uu;
                    AFFINE_LOOP();
#undef  AFFINE
                }

                break;
            }

            case DRAW_MODE_BLEND: {
                for (; y1 < y2; y1++, v1 += vv, dst += YETI_FRAMEBUFFER_WIDTH) {
                    u16 *tb = IMAGE_SCANLINE(src, f2i(v1));
                    u16 *fb = dst;
                    int i = x2 - x1, u = u1;
#define AFFINE(I) fb[I]=RGB_BLEND(fb[I],tb[f2i(u)]);u+=uu;
                    //#define AFFINE(I) if(XDITHER(I)^(y1&1))fb[I]=tb[f2i(u)];u+=uu;
                    AFFINE_LOOP();
#undef  AFFINE
                }

                break;
            }

            case DRAW_MODE_COPY:
            case DRAW_MODE_COPY_DITHER: { // Currently unsupported. Same as Copy.
                // TODO: Support memcpy16 optimization.
                for (; y1 < y2; y1++, v1 += vv, dst += YETI_FRAMEBUFFER_WIDTH) {
                    u16 *tb = IMAGE_SCANLINE(src, f2i(v1));
                    u16 *fb = dst;
                    int i = x2 - x1, u = u1;
#define AFFINE(I) fb[I]=tb[f2i(u)];u+=uu;
                    AFFINE_LOOP();
#undef  AFFINE
                }

                break;
            }
            }
        }
    }
}
#endif

/*
** Name: image_draw_at
** Desc:
*/
void image_draw_at(u16 *dst, u16 *src, int x, int y, int mode)
{
    image_draw(
        dst, x, y, x + IMAGE_WIDTH(src), y + IMAGE_HEIGHT(src),
        src, 0, 0, IMAGE_WIDTH(src), IMAGE_HEIGHT(src),
        mode);
}

/*
** Name: image_draw_di
** Desc: Draws a device independant image to the screen. The image will be
**       projected to match the viewport.
*/
void image_draw_di(
    u16 *dst, int x1, int y1, int x2, int y2, int z,
    u16 *src, int u1, int v1, int u2, int v2, int mode)
{
    if (z > 4) {
        vertex_t a, b;
        a.x = x1;
        a.y = y1;
        a.z = z;
        b.x = x2;
        b.y = y2;
        b.z = z;
        VERTEX_PROJECT_FAST(&a);
        VERTEX_PROJECT_FAST(&b);
        image_draw(
            dst, f2i(a.sx), f2i(a.sy), f2i(b.sx), f2i(b.sy),
            src, u1, v1, u2, v2, mode);
    }
}

