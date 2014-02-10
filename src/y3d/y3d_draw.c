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

#include "y3d_draw.h"
#include "y3d_image.h"

/******************************************************************************/

// Returns an index to a texture pixel.
//#define TEXEL(U,V) ((f2i(V)&63)<<6|(f2i(U)&63))
#define TEXEL64(U,V) ((u32)(V)<<18>>26<<6)|((u32)(U)<<18>>26)
#define TEXEL64DITHER(U,V,I) TEXEL64((U)+dk.u[XDITHER(I)],(V)+dk.v[XDITHER(I)])

//#define TEXEL128(U,V) (f2i(v)<<7)|f2i(u)
#define TEXEL128(U,V) ((u32)(V)<<17>>25<<7)|((u32)(U)<<17>>25)
#define TEXEL128DITHER(U,V,I) TEXEL128((U)+dk.u[XDITHER(I)],(V)+dk.v[XDITHER(I)])

// Returns an idex to a light array.
#define LIGHT(A) f2i(A)
#define LIGHTDITHER(A,I) LIGHT(l+dk.u[XDITHER(I)])

#define IS_CONSTANT_Z(A) ((A) < i2f(0) && (A) > -i2f(0))

// Dither kernals taken from:
// http://www.flipcode.com/cgi-bin/msg.cgi?showThread=10July2000-TexturingAsInUnreal&forum=askmid&id=-1

dither_kernal_t dither_kernals[2] = {
    {
        { 64, 128},
        {  0, 192}
    },
    {
        {192,   0},
        {128,  64}
    }
};

/*
** Name: fill_rect
** Desc:
*/
void fill_rect(pixel_buffer_t *dst, int x1, int y1, int x2, int y2, int pixel)
{
    int x;

    for (; y1 < y2; y1++) {
        for (x = x1; x < x2; x++) {
            dst->pixels[y1][x] = pixel;
        }
    }
}

/*
** Name: draw_line_nc
** Desc: Draw a unclipped line given a colour. Coords are 24:8 fixpt.
** Note: This function is very fast, but inaccurate. The clipper should
**       give at least 2+ pixels at the viewport edges to prevent buffer
**       overflow.
*/
void draw_line_nc(
    pixel_buffer_t *dst,
    int x1, int y1, int x2, int y2,
    int color)
{
    x2 -= x1;
    y2 -= y1;
    {
        int x = ABS(x2), y = ABS(y2), i = f2i(MAX(x, y));
        x2 = r2i(x2 * reciprocal[i]);
        y2 = r2i(y2 * reciprocal[i]);

        for (; i--; x1 += x2, y1 += y2) {
            dst->pixels[f2i(y1)][f2i(x1)] = color;
        }
    }
}

/*
** Name: draw_poly_line_nc
** Desc: Draw a unclipped polygon using lines of a given colour.
*/
void draw_poly_line_nc(
    pixel_buffer_t *dst,
    polyclip_t p, int n,
    int color)
{
    for (p[n] = p[0]; n--; p++) {
        int x1 = p[0]->sx;
        int y1 = p[0]->sy;
        int x2 = p[1]->sx;
        int y2 = p[1]->sy;
        draw_line_nc(dst,
                     CLAMP(x1, i2f(YETI_VIEWPORT_X1 + 2), i2f(YETI_VIEWPORT_X2 - 2)),
                     CLAMP(y1, i2f(YETI_VIEWPORT_Y1 + 2), i2f(YETI_VIEWPORT_Y2 - 2)),
                     CLAMP(x2, i2f(YETI_VIEWPORT_X1 + 2), i2f(YETI_VIEWPORT_X2 - 2)),
                     CLAMP(y2, i2f(YETI_VIEWPORT_Y1 + 2), i2f(YETI_VIEWPORT_Y2 - 2)),
                     color);
    }
}

/******************************************************************************/

/*
** Name: draw_char
** Desc:
*/
void draw_char(pixel_buffer_t *fb, int x, int y, int c, int xs, int ys, int mode)
{
    extern u16 spr_font[];
    int u = ((u8)c >> 4) << 3;
    int v = ((u8)c & 15) << 3;
    image_draw(fb->pixels[0],
               TEXT_COORD(x + 0, xs) + YETI_VIEWPORT_X1,
               TEXT_COORD(y + 0, ys) + YETI_VIEWPORT_Y1,
               TEXT_COORD(x + 1, xs) + YETI_VIEWPORT_X1,
               TEXT_COORD(y + 1, ys) + YETI_VIEWPORT_Y1,
               spr_font, u, v, u + 8, v + 8,
               mode);
}

/*
** Name: draw_string
** Desc:
*/
void draw_string(pixel_buffer_t *fb, int x, int y, char *s, int xs, int ys, int mode, int os)
{
    while (*s) {
        draw_char(fb, x++, y, *s++ + os, xs, ys, mode);
    }
}

/******************************************************************************/

/*
** Name: tmap_16_cs
** Desc: 16bit texture mapping using constant slopes. This function is designed
**       for slower processors.
*/
void tmap_16_cs(polygon_t p, int n, u16 *dst, u16 *src, int mode)
{
    int i = fixmul2(p[0].x - p[2].x, p[1].y - p[2].y) - fixmul2(p[1].x - p[2].x, p[0].y - p[2].y);

    if (i) {
        int uu = ((p[0].u - p[2].u) * (p[1].y - p[2].y) - (p[1].u - p[2].u) * (p[0].y - p[2].y)) / i;
        int vv = ((p[0].v - p[2].v) * (p[1].y - p[2].y) - (p[1].v - p[2].v) * (p[0].y - p[2].y)) / i;
        int x1, x2, lt_i, rt_i, lt_length = 0, rt_length = 0;
        int y1 = Y1_INIT, y2 = Y2_INIT, d[POLYGON_MAX];

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
            int lt_x, lt_xx, lt_u, lt_uu, lt_v, lt_vv;
            int rt_x, rt_xx;

            if (--lt_length <= 0) {
                do {
                    i = lt_i--;

                    if (lt_i < 0) {
                        lt_i = n - 1;
                    }
                } while ((lt_length = d[lt_i] - d[i]) <= 0);

#ifdef __WIN32__
                lt_xx = (p[lt_i].x - (lt_x = p[i].x)) / lt_length;
                lt_uu = (p[lt_i].u - (lt_u = p[i].u)) / lt_length;
                lt_vv = (p[lt_i].v - (lt_v = p[i].v)) / lt_length;
#else
                lt_xx = r2i((p[lt_i].x - (lt_x = p[i].x)) * reciprocal[lt_length]);
                lt_uu = r2i((p[lt_i].u - (lt_u = p[i].u)) * reciprocal[lt_length]);
                lt_vv = r2i((p[lt_i].v - (lt_v = p[i].v)) * reciprocal[lt_length]);
#endif
            }

            if (--rt_length <= 0) {
                do {
                    i = rt_i++;

                    if (rt_i >= n) {
                        rt_i = 0;
                    }
                } while ((rt_length = d[rt_i] - d[i]) <= 0);

#ifdef __WIN32__
                rt_xx = (p[rt_i].x - (rt_x = p[i].x)) / rt_length;
#else
                rt_xx = r2i((p[rt_i].x - (rt_x = p[i].x)) * reciprocal[rt_length]);
#endif
            }

            if ((x1 = fixup(lt_x)) < YETI_VIEWPORT_X1) {
                x1 = YETI_VIEWPORT_X1;
            }

            if ((x2 = fixup(rt_x)) > YETI_VIEWPORT_X2) {
                x2 = YETI_VIEWPORT_X2;
            }

            if ((i = x2 - x1) > 0) {
                u16 *fb = &dst[x1];
                int u = lt_u, v = lt_v;

                switch (mode) {
                case DRAW_MODE_COPY: {
#define AFFINE(I) fb[I]=src[TEXEL128(u,v)];u+=uu;v+=vv;
                    AFFINE_LOOP();
#undef  AFFINE
                    break;
                }

                case DRAW_MODE_COPY_HALFBRIGHT: {
#define AFFINE(I) fb[I]=RGB_HALFBRIGHT(src[TEXEL128(u,v)]);u+=uu;v+=vv;
                    AFFINE_LOOP();
#undef  AFFINE
                    break;
                }
                }
            }

            lt_x += lt_xx;
            lt_u += lt_uu;
            lt_v += lt_vv;
            rt_x += rt_xx;
        }
    }
}

/*
** Name: tmap_16
** Desc: Texture maps a model polygon. Textures are 16bit and variable sized.
**       This function can be used to texture map other types of objects, but
**       it is recomended that you make a model and render that. ie: if you
**       want a skybox, then create a skybox model.
*/
#ifndef __PATCH_TMAP_16__
void tmap_16(polygon_t p, int n, u16 *dst, u16 *src, int mode)
{
    int i, x1, x2, lt_i, rt_i, lt_length = 0, rt_length = 0;
    int y1 = Y1_INIT, y2 = Y2_INIT, d[POLYGON_MAX];

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
        int lt_x, lt_xx, lt_u, lt_uu, lt_v, lt_vv;
        int rt_x, rt_xx, rt_u, rt_uu, rt_v, rt_vv;

        if (--lt_length <= 0) {
            do {
                i = lt_i--;

                if (lt_i < 0) {
                    lt_i = n - 1;
                }
            } while ((lt_length = d[lt_i] - d[i]) <= 0);

            lt_xx = r2i((p[lt_i].x - (lt_x = p[i].x)) * reciprocal[lt_length]);
            lt_uu = r2i((p[lt_i].u - (lt_u = p[i].u)) * reciprocal[lt_length]);
            lt_vv = r2i((p[lt_i].v - (lt_v = p[i].v)) * reciprocal[lt_length]);
        }

        if (--rt_length <= 0) {
            do {
                i = rt_i++;

                if (rt_i >= n) {
                    rt_i = 0;
                }
            } while ((rt_length = d[rt_i] - d[i]) <= 0);

            rt_xx = r2i((p[rt_i].x - (rt_x = p[i].x)) * reciprocal[rt_length]);
            rt_uu = r2i((p[rt_i].u - (rt_u = p[i].u)) * reciprocal[rt_length]);
            rt_vv = r2i((p[rt_i].v - (rt_v = p[i].v)) * reciprocal[rt_length]);
        }

        if ((x1 = fixup(lt_x)) < YETI_VIEWPORT_X1) {
            x1 = YETI_VIEWPORT_X1;
        }

        if ((x2 = fixup(rt_x)) > YETI_VIEWPORT_X2) {
            x2 = YETI_VIEWPORT_X2;
        }

        if ((i = x2 - x1) > 0) {
            u16 *fb = &dst[x1];
            int u, uu = r2i((rt_u - (u = lt_u)) * reciprocal[i]);
            int v, vv = r2i((rt_v - (v = lt_v)) * reciprocal[i]);

            switch (mode) {
            case DRAW_MODE_COLORKEY: {
                int c;
#define AFFINE(I) if((c=src[TEXEL128(u,v)])!=0)fb[I]=c;u+=uu;v+=vv;
                AFFINE_LOOP();
#undef  AFFINE
                break;
            }

            case DRAW_MODE_BLEND_DITHER:
            case DRAW_MODE_BLEND: {
#define AFFINE(I) fb[I]=RGB_BLEND(fb[I],src[TEXEL128(u,v)]);u+=uu;v+=vv;
                //#define AFFINE(I) if (XDITHER(I)^(y1&1))fb[I]=src[TEXEL128(u,v)];u+=uu;v+=vv;
                AFFINE_LOOP();
#undef  AFFINE
                break;
            }

            case DRAW_MODE_COPY: {
#define AFFINE(I) fb[I]=src[TEXEL128(u,v)];u+=uu;v+=vv;
                AFFINE_LOOP();
#undef  AFFINE
                break;
            }

            case DRAW_MODE_COPY_DITHER: {
                dither_kernal_t dk = dither_kernals[y1 & 1];
#define AFFINE(I) fb[I]=src[TEXEL128DITHER(u,v,I)];u+=uu;v+=vv;
                AFFINE_LOOP();
#undef  AFFINE
                break;
            }

            case DRAW_MODE_COPY_HALFBRIGHT: {
#define AFFINE(I) fb[I]=RGB_HALFBRIGHT(src[TEXEL128(u,v)]);u+=uu;v+=vv;
                AFFINE_LOOP();
#undef  AFFINE
                break;
            }
            }
        }

        lt_x += lt_xx;
        lt_u += lt_uu;
        lt_v += lt_vv;
        rt_x += rt_xx;
        rt_u += rt_uu;
        rt_v += rt_vv;
    }
}
#endif

/******************************************************************************/

/*
** Name: draw_poly_flat
** Desc:
*/
#ifndef __PATCH_DRAW_FILLED_POLY__
void draw_poly_flat(
    polygon_t p, int n,
    u16 *dst,
    u16 color)
{
    int lt_i, lt_x, lt_xx;
    int rt_i, rt_x, rt_xx;
    int x1, x2, i;
    int lt_length = 0, rt_length = 0;
    int y1 = Y1_INIT, y2 = Y2_INIT, d[POLYGON_MAX];

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
        }

        if (--rt_length <= 0) {
            do {
                i = rt_i++;

                if (rt_i >= n) {
                    rt_i = 0;
                }
            } while ((rt_length = d[rt_i] - d[i]) <= 0);

            rt_xx = r2i((p[rt_i].x - (rt_x = p[i].x)) * reciprocal[rt_length]);
        }

        if ((x1 = fixup(lt_x)) < YETI_VIEWPORT_X1) {
            x1 = YETI_VIEWPORT_X1;
        }

        if ((x2 = fixup(rt_x)) > YETI_VIEWPORT_X2) {
            x2 = YETI_VIEWPORT_X2;
        }

        if ((i = x2 - x1) > 0) {
            u16 *fb = &dst[x1];
#define AFFINE(I) fb[I] = color;
            AFFINE_LOOP();
#undef  AFFINE
        }

        lt_x += lt_xx;
        rt_x += rt_xx;
    }
}
#endif

/******************************************************************************/

/*
** Name: tmap_16_sky
** Desc: Interlaced sky bitmap renderer. The sky bitmap is draw withing the
**       given polygon. X Offsets are avaliable for each sky layer.
*/
void tmap_16_sky(
    polygon_t p, int n,
    u16 *dst,
    u16 *src1, int xos1,
    u16 *src2, int xos2)
{
    int lt_i, lt_x, lt_xx;
    int rt_i, rt_x, rt_xx;
    int x1, x2, i;
    int lt_length = 0, rt_length = 0;
    int y1 = Y1_INIT, y2 = Y2_INIT, d[POLYGON_MAX];

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
        }

        if (--rt_length <= 0) {
            do {
                i = rt_i++;

                if (rt_i >= n) {
                    rt_i = 0;
                }
            } while ((rt_length = d[rt_i] - d[i]) <= 0);

            rt_xx = r2i((p[rt_i].x - (rt_x = p[i].x)) * reciprocal[rt_length]);
        }

        if ((x1 = fixup(lt_x)) < YETI_VIEWPORT_X1) {
            x1 = YETI_VIEWPORT_X1;
        }

        if ((x2 = fixup(rt_x)) > YETI_VIEWPORT_X2) {
            x2 = YETI_VIEWPORT_X2;
        }

        if ((i = x2 - x1) > 0) {
            if (y1 & 1) {
                memcpy16(&dst[x1], &IMAGE_PIXEL(src1, x1 + xos1, BITSHF(y1, TEXT_SCALE)), i);
            } else {
                memcpy16(&dst[x1], &IMAGE_PIXEL(src2, x1 + xos2, BITSHF(y1, TEXT_SCALE)), i);
            }
        }

        lt_x += lt_xx;
        rt_x += rt_xx;
    }
}

/******************************************************************************/

/*
** Name: texcoord_fixup
** Desc: Fix up texture UV's so they are within optimal range. The PSONE
**       doesn't support texture wrapping, so we have to shift the UV's
**       withing range. Plus, excessive UV's increase fixpt errors.
*/
void texcoord_fixup(polygon_t p, int n)
{
    int i, u = p[0].u, v = p[0].v;

    for (i = 1; i < n; i++) {
        if (p[i].u < u) {
            u = p[i].u;
        }

        if (p[i].v < v) {
            v = p[i].v;
        }
    }

    for (i = n; i--;) {
        p[i].u -= u & ~(i2f(TEXTURE_WIDTH) - 1);
        p[i].v -= v & ~(i2f(TEXTURE_HEIGHT) - 1);
    }
}

/******************************************************************************/



#define AFFINE_LOOP_PC()\
  if (IS_CONSTANT_Z(zz))\
  {\
    AFFINE_LOOP();\
  }\
  else\
  {\
    for (; i > 16; i -= 16, fb += 16)\
    {\
      AFFINE( 0) AFFINE( 1) AFFINE( 2) AFFINE( 3)\
      AFFINE( 4) AFFINE( 5) AFFINE( 6) AFFINE( 7)\
      AFFINE( 8) AFFINE( 9) AFFINE(10) AFFINE(11)\
      AFFINE(12) AFFINE(13) AFFINE(14) AFFINE(15)\
      u += uu; v += vv; z += zz;\
      if (z)\
      {\
        aa = (FIXDIVXX(u, z) - a) >> 4;\
        bb = (FIXDIVXX(v, z) - b) >> 4;\
      }\
      else\
      {\
        aa = (0 - a) >> 4;\
        bb = (0 - b) >> 4;\
      }\
    }\
    switch (i)\
    {\
      AFFINE_CASE(16) AFFINE_CASE(15) AFFINE_CASE(14) AFFINE_CASE(13)\
      AFFINE_CASE(12) AFFINE_CASE(11) AFFINE_CASE(10) AFFINE_CASE( 9)\
      AFFINE_CASE( 8) AFFINE_CASE( 7) AFFINE_CASE( 6) AFFINE_CASE( 5)\
      AFFINE_CASE( 4) AFFINE_CASE( 3) AFFINE_CASE( 2) AFFINE_CASE( 1)\
    }\
  }

/*
** Name: tmap_8_horz_pc
** Desc: Renders a lit, texture mapped polygon to a viewport.
*/
#ifndef __PATCH_TMAP_8_HORZ_PC__
void tmap_8_horz_pc(polygon_t p, int n, u16 *dst, u8 *src, lua_t lua, int mode)
{
    int i, x1, x2, lt_i, rt_i, lt_length = 0, rt_length = 0;
    int y1 = Y1_INIT, y2 = Y2_INIT, d[POLYGON_MAX];
    texcoord_fixup(p, n);

    for (i = n; i--;) {
        if (p[i].z) {
            p[i].u = fixdiv64(p[i].u << 10, p[i].z);
            p[i].v = fixdiv64(p[i].v << 10, p[i].z);
            p[i].z = fixdiv64(FIXONE << 10, p[i].z);
        } else {
            p[i].u = p[i].v = 0;
        }

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
        int lt_x, lt_xx, lt_u, lt_uu, lt_v, lt_vv, lt_l, lt_ll, lt_z, lt_zz;
        int rt_x, rt_xx, rt_u, rt_uu, rt_v, rt_vv, rt_l, rt_ll, rt_z, rt_zz;
        int a, aa, b, bb;

        if (--lt_length <= 0) {
            do {
                i = lt_i--;

                if (lt_i < 0) {
                    lt_i = n - 1;
                }
            } while ((lt_length = d[lt_i] - d[i]) <= 0);

            lt_xx = (p[lt_i].x - (lt_x = p[i].x)) / lt_length;
            lt_uu = (p[lt_i].u - (lt_u = p[i].u)) / lt_length;
            lt_vv = (p[lt_i].v - (lt_v = p[i].v)) / lt_length;
            lt_ll = (p[lt_i].l - (lt_l = p[i].l)) / lt_length;
            lt_zz = (p[lt_i].z - (lt_z = p[i].z)) / lt_length;
        }

        if (--rt_length <= 0) {
            do {
                i = rt_i++;

                if (rt_i >= n) {
                    rt_i = 0;
                }
            } while ((rt_length = d[rt_i] - d[i]) <= 0);

            rt_xx = (p[rt_i].x - (rt_x = p[i].x)) / rt_length;
            rt_uu = (p[rt_i].u - (rt_u = p[i].u)) / rt_length;
            rt_vv = (p[rt_i].v - (rt_v = p[i].v)) / rt_length;
            rt_ll = (p[rt_i].l - (rt_l = p[i].l)) / rt_length;
            rt_zz = (p[rt_i].z - (rt_z = p[i].z)) / rt_length;
        }

        if ((x1 = fixup(lt_x)) < YETI_VIEWPORT_X1) {
            x1 = YETI_VIEWPORT_X1;
        }

        if ((x2 = fixup(rt_x)) > YETI_VIEWPORT_X2) {
            x2 = YETI_VIEWPORT_X2;
        }

        if ((i = x2 - x1) > 0) {
            u16 *fb = &dst[x1];
            int l, ll = (rt_l - (l = lt_l)) / i;
            int u, uu = (rt_u - (u = lt_u)) / i;
            int v, vv = (rt_v - (v = lt_v)) / i;
            int z, zz = (rt_z - (z = lt_z)) / i;
            uu <<= 4;
            vv <<= 4;
            zz <<= 4;

            if (z) {
                a = FIXDIVXX(u, z);
                b = FIXDIVXX(v, z);
            } else {
                a = b = 0;
            }

            u += uu;
            v += vv;
            z += zz;

            if (z) {
                aa = (FIXDIVXX(u, z) - a) >> 4;
                bb = (FIXDIVXX(v, z) - b) >> 4;
            } else {
                aa = (0 - a) >> 4;
                bb = (0 - b) >> 4;
            }

            switch (mode) {
            case DRAW_MODE_COLORKEY: {
                int c;
#define AFFINE(I) if((c=src[TEXEL64(a,b)])!=0)fb[I]=lua[LIGHT(l)][c];a+=aa;b+=bb;l+=ll;
                AFFINE_LOOP_PC();
#undef AFFINE
                break;
            }

            case DRAW_MODE_BLEND: {
#define AFFINE(I) fb[I]=RGB_BLEND(fb[I],lua[LIGHT(l)][src[TEXEL64(a,b)]]);a+=aa;b+=bb;l+=ll;
                //#define AFFINE(I) if (XDITHER(I)^(y1&1))fb[I]=lua[LIGHT(l)][src[TEXEL64(a,b)]];a+=aa;b+=bb;l+=ll;
                AFFINE_LOOP_PC();
#undef AFFINE
                break;
            }

            case DRAW_MODE_COPY: {
#define AFFINE(I) fb[I]=lua[LIGHT(l)][src[TEXEL64(a,b)]];a+=aa;b+=bb;l+=ll;
                AFFINE_LOOP_PC();
#undef AFFINE
                break;
            }

            case DRAW_MODE_COPY_DITHER: {
                dither_kernal_t dk = dither_kernals[y1 & 1];
#define AFFINE(I) fb[I]=lua[LIGHTDITHER(l,I)][src[TEXEL64DITHER(a,b,I)]];a+=aa;b+=bb;l+=ll;
                AFFINE_LOOP_PC();
#undef AFFINE
                break;
            }

            case DRAW_MODE_BLEND_DITHER: {
                dither_kernal_t dk = dither_kernals[y1 & 1];
#define AFFINE(I) fb[I]=RGB_BLEND(fb[I],lua[LIGHTDITHER(l,I)][src[TEXEL64DITHER(a,b,I)]]);a+=aa;b+=bb;l+=ll;
                AFFINE_LOOP_PC();
#undef AFFINE
                break;
            }
            }
        }

        lt_x += lt_xx;
        lt_u += lt_uu;
        lt_v += lt_vv;
        lt_l += lt_ll;
        lt_z += lt_zz;
        rt_x += rt_xx;
        rt_u += rt_uu;
        rt_v += rt_vv;
        rt_l += rt_ll;
        rt_z += rt_zz;
    }
}
#endif

/******************************************************************************/

/*
** Name: tmap_8_horz
** Desc: Renders a lit, texture mapped polygon to a viewport.
*/
#ifndef __PATCH_TMAP_8_HORZ__
void tmap_8_horz(polygon_t p, int n, u16 *dst, u8 *src, lua_t lua, int mode)
{
    int i, x1, x2, lt_i, rt_i, lt_length = 0, rt_length = 0;
    int y1 = Y1_INIT, y2 = Y2_INIT, d[POLYGON_MAX];

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
        int lt_x, lt_xx, lt_u, lt_uu, lt_v, lt_vv, lt_l, lt_ll;
        int rt_x, rt_xx, rt_u, rt_uu, rt_v, rt_vv, rt_l, rt_ll;

        if (--lt_length <= 0) {
            do {
                i = lt_i--;

                if (lt_i < 0) {
                    lt_i = n - 1;
                }
            } while ((lt_length = d[lt_i] - d[i]) <= 0);

            lt_xx = r2i((p[lt_i].x - (lt_x = p[i].x)) * reciprocal[lt_length]);
            lt_uu = r2i((p[lt_i].u - (lt_u = p[i].u)) * reciprocal[lt_length]);
            lt_vv = r2i((p[lt_i].v - (lt_v = p[i].v)) * reciprocal[lt_length]);
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
            rt_uu = r2i((p[rt_i].u - (rt_u = p[i].u)) * reciprocal[rt_length]);
            rt_vv = r2i((p[rt_i].v - (rt_v = p[i].v)) * reciprocal[rt_length]);
            rt_ll = r2i((p[rt_i].l - (rt_l = p[i].l)) * reciprocal[rt_length]);
        }

        if ((x1 = fixup(lt_x)) < YETI_VIEWPORT_X1) {
            x1 = YETI_VIEWPORT_X1;
        }

        if ((x2 = fixup(rt_x)) > YETI_VIEWPORT_X2) {
            x2 = YETI_VIEWPORT_X2;
        }

        if ((i = x2 - x1) > 0) {
            u16 *fb = &dst[x1];
            int l = lt_l, ll = r2i((rt_l - lt_l) * reciprocal[i]);
            int a = lt_u, aa = r2i((rt_u - lt_u) * reciprocal[i]);
            int b = lt_v, bb = r2i((rt_v - lt_v) * reciprocal[i]);

            switch (mode) {
            case DRAW_MODE_COLORKEY: {
                int c;
#define AFFINE(I) if((c=src[TEXEL64(a,b)])!=0)fb[I]=lua[LIGHT(l)][c];a+=aa;b+=bb;l+=ll;
                AFFINE_LOOP();
#undef  AFFINE
                break;
            }

            case DRAW_MODE_BLEND_DITHER:
            case DRAW_MODE_BLEND: {
#define AFFINE(I) fb[I]=RGB_BLEND(fb[I],lua[LIGHT(l)][src[TEXEL64(a,b)]]);a+=aa;b+=bb;l+=ll;
                //#define AFFINE(I) if (XDITHER(I)^(y1&1))fb[I]=lua[LIGHT(l)][src[TEXEL64(a,b)]];a+=aa;b+=bb;l+=ll;
                AFFINE_LOOP();
#undef AFFINE
                break;
            }

            case DRAW_MODE_COPY_DITHER:
            case DRAW_MODE_COPY: {
#define AFFINE(I) fb[I]=lua[LIGHT(l)][src[TEXEL64(a,b)]];a+=aa;b+=bb;l+=ll;
                AFFINE_LOOP();
#undef  AFFINE
                break;
            }
            }
        }

        lt_x += lt_xx;
        lt_u += lt_uu;
        lt_v += lt_vv;
        lt_l += lt_ll;
        rt_x += rt_xx;
        rt_u += rt_uu;
        rt_v += rt_vv;
        rt_l += rt_ll;
    }
}
#endif




