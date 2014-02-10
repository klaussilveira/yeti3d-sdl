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


#include "y3d_pixel.h"
#include "y3d_draw.h"
#include "y3d_image.h"

/******************************************************************************/

/*
** Name: pixel_converter_init
** Desc:
*/
void pixel_converter_init(
    pixel_converter_t *pixel_converter,
    int rmask, int gmask, int bmask,
    int filter)
{
    int i;
    CLEARMEM(pixel_converter);
    pixel_converter->length = STATICARRAY_MAX(pixel_converter);

    for (i = pixel_converter->length; i--;) {
        if (filter) {
            int r = RGB_RED(i);
            int g = RGB_GREEN(i);
            int b = RGB_BLUE(i);

            switch (filter) {
            case PIXEL_FILTER_GOLDEN:
                g = (g + r) >> 1;
                break;

            case PIXEL_FILTER_MURKY:
                r = (r + g) >> 1;
                break;

            case PIXEL_FILTER_BROWN:
                g = (b + r) >> 1;
                break;

            case PIXEL_FILTER_HORROR:
                g = b = RGB_AVERAGE(r, g, b);
                break;

            case PIXEL_FILTER_GRAYSCALE:
                r = g = b = RGB_AVERAGE(r, g, b);
                break;
            }

            pixel_converter->item[i] = PIXEL_CONVERT(RGB_SET(r, g, b), rmask, gmask, bmask);
        } else {
            pixel_converter->item[i] = PIXEL_CONVERT(i, rmask, gmask, bmask);
        }
    }
}

/*
** Name: pixel555_convert
** Desc: Used to convert 555 lookup tables and sprites to 565
*/
int pixel555_convert(int color, int rmask, int gmask, int bmask)
{
    return
        (((((color) >> 0) & 31) * rmask / 31) & rmask) |
        (((((color) >> 5) & 31) * gmask / 31) & gmask) |
        (((((color) >> 10) & 31) * bmask / 31) & bmask) ;
}

/*
** Name: pixel555_convert_array
** Desc:
*/
void pixel555_convert_array(u16 *pixels, int length, int rmask, int gmask, int bmask)
{
    while (length--) {
        *pixels++;
        *pixels = pixel555_convert(*pixels, rmask, gmask, bmask);
    }
}

/*
** Name: pixel_convert
** Desc: Converts the default pixel format to another.
*/
int pixel_convert(int color, int rmask, int gmask, int bmask)
{
    return PIXEL_CONVERT(color, rmask, gmask, bmask);
}

/*
** Name: pixel_convert_array
** Desc:
*/
void pixel_convert_array(u16 *pixels, int length, int rmask, int gmask, int bmask)
{
    while (length--) {
        *pixels++;
        *pixels = PIXEL_CONVERT(*pixels, rmask, gmask, bmask);
    }
}

/*
** Name: pixel_convert_image
** Desc:
*/
void pixel_convert_image(u16 *image, int rmask, int gmask, int bmask)
{
    pixel_convert_array(
        IMAGE_SCANLINE(image, 0),
        IMAGE_WIDTH(image) * IMAGE_HEIGHT(image),
        rmask, gmask, bmask);
}

/******************************************************************************/

/*
** Name: pixel_buffer_draw
** Desc: The Yeti3D uses 5:5:5 colour, so if your platform uses a different
**       format, then one solution is to convert the pixels when blitting to
**       the video buffer.
** Todo: Should use a standard memcpy16 if a pixel_converter is not supplied.
*/
void pixel_buffer_draw(
    pixel_buffer_t *pixelbuffer,
    u16 *video, int pitch,
    pixel_converter_t *pixel_converter)
{
    if (pixel_converter) {
        int y;

        for (y = 0; y < YETI_FRAMEBUFFER_HEIGHT; y++) {
            u16 *fb = video;
            u16 *tb = pixelbuffer->pixels[y];
            int i = YETI_FRAMEBUFFER_WIDTH;
#define AFFINE(I) fb[I] = pixel_converter->item[*tb++];
            AFFINE_LOOP()
#undef AFFINE
            video = (u16 *)((int)video + pitch);
        }
    }
}

/*
** Name: pixel_buffer_clear
** Desc: Clears a pixel buffer to black.
*/
void pixel_buffer_clear(pixel_buffer_t *pixel_buffer)
{
    memset(pixel_buffer, 0, sizeof(pixel_buffer_t));
}
