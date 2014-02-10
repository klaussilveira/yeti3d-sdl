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

#include "y3d_surface.h"
#include "y3d_draw.h"

/******************************************************************************/

/*
** Name: surface_init
** Desc: Initilize surface data.
*/
void surface_init(surface_t *surface)
{
    CLEARMEM(surface);
    surface->xsize = SURFACE_DEFAULT_SIZE;
    surface->ysize = SURFACE_DEFAULT_SIZE;
    surface->mode  = DRAW_MODE_COPY;
}

/*
** Name: surface_array_init
** Desc: Initilize surfaces data.
*/
void surface_array_init(surface_array_t *surfaces)
{
    int i;
    surfaces->length = SURFACE_MAX;

    for (i = 0; i < surfaces->length; i++) {
        surface_init(&surfaces->item[i]);
    }

#define SURFACE_ADJUST(I,X,Y) {surfaces->item[I].xsize+=(X); surfaces->item[I].ysize+=(Y);}
#define SURFACE(A,B) SURFACE_ADJUST(A,B,B)
    SURFACE_ADJUST(TEXTURE_WATER  ,  1,  1); //surfaces->item[TEXTURE_WATER].mode = DRAW_MODE_BLEND;
    SURFACE_ADJUST(TEXTURE_LAVA   ,  0,  0); //surfaces->item[TEXTURE_LAVA ].mode = DRAW_MODE_BLEND;
    SURFACE_ADJUST(TEXTURE_SKY    , -1, -1);
    SURFACE_ADJUST(TEXTURE_LIGHT2 ,  2,  3);
    SURFACE_ADJUST(23, -1, 0);
    SURFACE_ADJUST(74,  1, 2);
    SURFACE_ADJUST(76,  1, 0);
    SURFACE_ADJUST(77,  1, 0);
    SURFACE_ADJUST(78,  1, 0);
    SURFACE_ADJUST(79,  1, 0);
    SURFACE_ADJUST(92,  1, 0);
    SURFACE(2, 1);
    SURFACE(4, 0);
    //  SURFACE_ADJUST(10, -1, 0);
    SURFACE(11, 1);
    SURFACE(13, 1);
    SURFACE(15, -1);
    SURFACE(28, 1);
    SURFACE_ADJUST(29, 1, 0);
    SURFACE(43, 0);
    SURFACE(51, 1);
    SURFACE(53, 1);
    SURFACE(57, -1);
    SURFACE(69, 2);
    SURFACE(80, 1);
    SURFACE(81, 2);
    SURFACE(82, 1);
    SURFACE(84, 1);
    SURFACE(86, 2);
    SURFACE(87, 1);
    SURFACE(89, 1);
    SURFACE(90, 2);
    //  SURFACE( 91, 1);
    SURFACE_ADJUST(94, 1, 2);
    SURFACE(95, 1);
    SURFACE(96, 2);
    SURFACE(97, 0);
    SURFACE(101, 1);
    SURFACE(104, 2);
    SURFACE(105, 2);
    SURFACE(106, 2);
    SURFACE(107, 2);
    SURFACE(108, 2);
    SURFACE(111, 1);
    SURFACE(113, 1);
    SURFACE(115, 2);
    SURFACE(116, 2);
    SURFACE(117, 2);
    SURFACE_ADJUST(119,  2, 1);
    SURFACE_ADJUST(120,  1, 1);
    SURFACE(121, 3);
    surfaces->item[121].mode = DRAW_MODE_COLORKEY;
    SURFACE(122, 1);
    surfaces->item[122].mode = DRAW_MODE_COLORKEY;
    SURFACE(123, 1);
    SURFACE(124, 2);
    surfaces->item[124].mode = DRAW_MODE_COLORKEY;
    SURFACE(125, 2);
    surfaces->item[125].mode = DRAW_MODE_BLEND;
    SURFACE_ADJUST(127, 1, 1);
}
