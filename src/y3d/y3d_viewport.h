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

#ifndef __Y3D_VIEWPORT_H__
#define __Y3D_VIEWPORT_H__

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

/*
** Some Porting Notes:
**
** - The standard pixel format is 5:5:5. There is a function in yeti.c which
**   will copy the viewport to a video display that supports a different pixel
**   format.
** - You can patch functions by using the __PATCH?????__ macros.
** - Make sure you define the platform macro in your IDE/make scripts.
** - Viewports larger than 320x240 are not recommended unless you patch the
**   default texture mapper. The standard DDA code is limited to small displays.
**   Look at the OpenGL port for how todo this.
**
*/

/******************************************************************************/

#ifdef __GBA__

#define YETI_RGB555
#define YETI_LOW_QUALITY_PROJECTION
#define YETI_LOW_QUALITY_CLIPPING

#define YETI_VIEWPORT_INTERVAL 800
#define YETI_VIEWPORT_INTERVAL_ERROR 200

#ifdef __FRUITY__
#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 240
#define YETI_VIEWPORT_Y2 160
#define YETI_FRAMEBUFFER_WIDTH 240
#define YETI_FRAMEBUFFER_HEIGHT 160
#else
#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 160
#define YETI_VIEWPORT_Y2 128
#define YETI_FRAMEBUFFER_WIDTH 160
#define YETI_FRAMEBUFFER_HEIGHT 128
#endif

#endif

/******************************************************************************/

#ifdef __SERIES60__

#define YETI_RGB444

#define YETI_VIEWPORT_INTERVAL 40
#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 176
#define YETI_VIEWPORT_Y2 208
#define YETI_FRAMEBUFFER_WIDTH 176
#define YETI_FRAMEBUFFER_HEIGHT 208

#endif

/******************************************************************************/

#ifdef __GP32__

#define YETI_RGB565

#define YETI_VIEWPORT_INTERVAL 40
#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 320
#define YETI_VIEWPORT_Y2 240
#define YETI_FRAMEBUFFER_WIDTH 320
#define YETI_FRAMEBUFFER_HEIGHT 240

#endif

/******************************************************************************/

#ifdef __POCKETPC__

#define YETI_RGB565
#define YETI_VIEWPORT_INTERVAL 40

/*
#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 320
#define YETI_VIEWPORT_Y2 240
#define YETI_FRAMEBUFFER_WIDTH 320
#define YETI_FRAMEBUFFER_HEIGHT 240
*/

#define YETI_FRAMEBUFFER_MAXSIZE 320 * 240

extern int YetiViewPortX1;
extern int YetiViewPortY1;
extern int YetiViewPortX2;
extern int YetiViewPortY2;
extern int YetiFrameBufferWidth;
extern int YetiFrameBufferHeight;

#define YETI_VIEWPORT_X1 YetiViewPortX1
#define YETI_VIEWPORT_Y1 YetiViewPortY1
#define YETI_VIEWPORT_X2 YetiViewPortX2
#define YETI_VIEWPORT_Y2 YetiViewPortY2
#define YETI_FRAMEBUFFER_WIDTH YetiFrameBufferWidth
#define YETI_FRAMEBUFFER_HEIGHT YetiFrameBufferHeight

#endif

/******************************************************************************/

#ifdef __SVGALIB__

#define YETI_RGB555

#define YETI_VIEWPORT_INTERVAL 40
#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 320
#define YETI_VIEWPORT_Y2 240
#define YETI_FRAMEBUFFER_WIDTH 320
#define YETI_FRAMEBUFFER_HEIGHT 240

#endif

/******************************************************************************/

#ifdef __GAMECUBE__

#define YETI_RGB565

#define YETI_VIEWPORT_INTERVAL 40
#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 320
#define YETI_VIEWPORT_Y2 240
#define YETI_FRAMEBUFFER_WIDTH 320
#define YETI_FRAMEBUFFER_HEIGHT 240

#endif

/******************************************************************************/

#ifdef __WIN32__

#define YETI_RGB555

#define YETI_VIEWPORT_INTERVAL 37

#ifdef __DDRAW__

//#define YETI_LOW_QUALITY_CLIPPING
//#define YETI_LOW_QUALITY_PROJECTION

#define YETI_FRAMEBUFFER_WIDTH 800
#define YETI_FRAMEBUFFER_HEIGHT 600
#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 YETI_FRAMEBUFFER_WIDTH
#define YETI_VIEWPORT_Y2 YETI_FRAMEBUFFER_HEIGHT
#define TEXT_SCALE 2

#else

#ifdef __FRUITY__

#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 240
#define YETI_VIEWPORT_Y2 160
#define YETI_FRAMEBUFFER_WIDTH 240
#define YETI_FRAMEBUFFER_HEIGHT 160

#else

#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 320
#define YETI_VIEWPORT_Y2 240
#define YETI_FRAMEBUFFER_WIDTH 320
#define YETI_FRAMEBUFFER_HEIGHT 240

#endif
#endif

#endif

/******************************************************************************/

#ifdef __OPENGL__

#define YETI_RGB555

//#define YETI_VIEWPORT_INTERVAL 35

#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 320
#define YETI_VIEWPORT_Y2 240
#define YETI_FRAMEBUFFER_WIDTH 320
#define YETI_FRAMEBUFFER_HEIGHT 240

#define __PATCH_YETI_CLEAR_VIEWPORT__
#define __PATCH_IMAGE_DRAW__
#define __PATCH_MODEL_POLY__
#define __PATCH_DRAW_TEXTURE__
//#define __PATCH_SOUND_PLAY__

#endif

/******************************************************************************/

#ifdef __MSDOS__

#define YETI_RGB555

#define YETI_VIEWPORT_INTERVAL 40
#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 320
#define YETI_VIEWPORT_Y2 240
#define YETI_FRAMEBUFFER_WIDTH 320
#define YETI_FRAMEBUFFER_HEIGHT 240

#endif

/******************************************************************************/

#ifdef __PSONE__

#define YETI_RGB555

#define YETI_VIEWPORT_INTERVAL 3
#define YETI_VIEWPORT_INTERVAL_ERROR 1

#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 320
#define YETI_VIEWPORT_Y2 240
#define YETI_FRAMEBUFFER_WIDTH 320
#define YETI_FRAMEBUFFER_HEIGHT 240

#define __PATCH_DRAW_CLIPPED_POLY__

#endif

/******************************************************************************/

#ifdef __SDL__

#define YETI_RGB555
#define YETI_GAMMA (2.1)

#define YETI_FRAMEBUFFER_WIDTH 640
#define YETI_FRAMEBUFFER_HEIGHT 480
#define YETI_VIEWPORT_X1 0
#define YETI_VIEWPORT_Y1 0
#define YETI_VIEWPORT_X2 YETI_FRAMEBUFFER_WIDTH
#define YETI_VIEWPORT_Y2 YETI_FRAMEBUFFER_HEIGHT
#define YETI_VIEWPORT_INTERVAL 35
#define TEXT_SCALE 2

#endif

/******************************************************************************/

#ifdef YETI_RGB555
#define RGB_BLEND_MASK  0x7BDE
#define RGB_SET(R,G,B)  (((B)<<10)|((G)<<5)|(R))
#define RGB_RED(A)      (((A)>> 0)&31)
#define RGB_GREEN(A)    (((A)>> 5)&31)
#define RGB_BLUE(A)     (((A)>>10)&31)
#endif

#ifdef YETI_RGB444
#define RGB_BLEND_MASK  0x0EEE
#define RGB_SET(R,G,B)  ((((R)>>1)<<8)|(((G)>>1)<<4)|((B)>>1))
#define RGB_RED(A)      ((((A)>>8)<<1)&31)
#define RGB_GREEN(A)    ((((A)>>4)<<1)&31)
#define RGB_BLUE(A)     ((((A)>>0)<<1)&31)
#endif

#ifdef YETI_RGB565
#define RGB_BLEND_MASK  0xF7DE
#define RGB_SET(R,G,B)  (((R) << 11) | ((G) << 6) | ((B) >> 0))
#define RGB_RED(A)      (((A)>> 0)&31)
#define RGB_GREEN(A)    (((A)>> 6)&31)
#define RGB_BLUE(A)     (((A)>>11)&31)
#endif

/******************************************************************************/

#define YETI_VIEWPORT_WIDTH   (YETI_VIEWPORT_X2-YETI_VIEWPORT_X1)
#define YETI_VIEWPORT_HEIGHT  (YETI_VIEWPORT_Y2-YETI_VIEWPORT_Y1)
#define YETI_VIEWPORT_XCENTER ((YETI_VIEWPORT_X1+YETI_VIEWPORT_X2)>>1)
#define YETI_VIEWPORT_YCENTER ((YETI_VIEWPORT_Y1+YETI_VIEWPORT_Y2)>>1)
#define YETI_VIEWPORT_RECT    YETI_VIEWPORT_X1, YETI_VIEWPORT_Y1, YETI_VIEWPORT_X2, YETI_VIEWPORT_Y2

#define YETI_TEXTPORT_WIDTH   (YETI_VIEWPORT_WIDTH>>3)
#define YETI_TEXTPORT_HEIGHT  (YETI_VIEWPORT_HEIGHT>>3)

#ifndef TEXT_SCALE
#define TEXT_SCALE 0
#endif

#define TEXT_COORD(X,SCALE) ((X)<<((SCALE)+TEXT_SCALE))

/******************************************************************************/

#define COLOR_WHITE RGB_SET(31, 31, 31)
#define COLOR_LIME  RGB_SET( 0, 31,  0)
#define COLOR_RED   RGB_SET(31,  0,  0)

#define RGB_SET_CLAMP(R,G,B)  RGB_SET(CLAMP(R,0,31),CLAMP(G,0,31),CLAMP(B,0,31));
#define RGB_AVERAGE(R,B,G)    f2i(((R)*77)+((G)*150)+((B)*27))
#define RGB_BLEND(A,B)        ((((A)&RGB_BLEND_MASK)+((B)&RGB_BLEND_MASK))>>1)
#define RGB_HALFBRIGHT(A)     (((A)&RGB_BLEND_MASK)>>1)

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif


