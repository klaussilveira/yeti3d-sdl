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

#ifndef __Y3D_PIXEL_H__
#define __Y3D_PIXEL_H__

/******************************************************************************/

#include "y3d_ansic.h"
#include "y3d_viewport.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define FRAMEBUFFER_WIDTH YETI_FRAMEBUFFER_WIDTH
#define FRAMEBUFFER_HEIGHT YETI_FRAMEBUFFER_HEIGHT

#define PIXEL_CONVERT(COLOR,RMASK,GMASK,BMASK) \
    ((RGB_RED  (COLOR) * (RMASK) / 31) & (RMASK)) | \
    ((RGB_GREEN(COLOR) * (GMASK) / 31) & (GMASK)) | \
    ((RGB_BLUE (COLOR) * (BMASK) / 31) & (BMASK)) ;

/******************************************************************************/

#define PIXEL_FILTER_NORMAL     0
#define PIXEL_FILTER_GOLDEN     1
#define PIXEL_FILTER_MURKY      2
#define PIXEL_FILTER_BROWN      3
#define PIXEL_FILTER_HORROR     4
#define PIXEL_FILTER_GRAYSCALE  5

/******************************************************************************/

typedef u16 pixel_t;
typedef STATICARRAY(pixel_t, 65536) pixel_converter_t;

typedef struct pixel_buffer_t {
#ifdef __POCKETPC__
    pixel_t pixels[1][YETI_FRAMEBUFFER_MAXSIZE]; /// Sean mod
#else
    pixel_t pixels[FRAMEBUFFER_HEIGHT][FRAMEBUFFER_WIDTH];
#endif
} pixel_buffer_t;

typedef struct viewport_t {
    pixel_buffer_t *front;
    pixel_buffer_t *back;
} viewport_t;

/******************************************************************************/

extern int pixel555_convert(int color, int rmask, int gmask, int bmask);
extern void pixel555_convert_array(u16 *pixels, int length, int rmask, int gmask, int bmask);
extern void pixel_converter_init(pixel_converter_t *pixel_converter, int rmask, int gmask, int bmask, int filter);
extern void pixel_convert_image(u16 *image, int rmask, int gmask, int bmask);
extern void pixel_buffer_draw(
    pixel_buffer_t *pixelbuffer,
    u16 *video, int pitch,
    pixel_converter_t *pixel_converter);

extern void pixel_buffer_clear(pixel_buffer_t *pixel_buffer);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif
