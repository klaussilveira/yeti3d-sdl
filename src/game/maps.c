/*
** Name: Yeti3D
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

#include "maps.h"

/******************************************************************************/

#define YETI_ROM

#include "maps/e1m1.c"
#include "maps/e1m2.c"
#include "maps/e1m3.c"
#include "maps/e1m4.c"
#include "maps/e1m5.c"
#include "maps/e1m6.c"
#include "maps/e1m7.c"
#include "maps/e1m8.c"
#include "maps/e1m9.c"

#include "maps/e2m1.c"
#include "maps/e2m2.c"
#include "maps/e2m3.c"
#include "maps/e2m4.c"
#include "maps/e2m5.c"
#include "maps/e2m6.c"
#include "maps/e2m7.c"
#include "maps/e2m8.c"
#include "maps/e2m9.c"

#include "maps/e3m1.c"
#include "maps/e3m2.c"
#include "maps/e3m3.c"
#include "maps/e3m4.c"
#include "maps/e3m5.c"
#include "maps/e3m6.c"
#include "maps/e3m7.c"
#include "maps/e3m8.c"
#include "maps/e3m9.c"

rom_map_t *maps[] = {
    &map_e2m3,
    &map_e1m9, &map_e2m1, &map_e1m8, &map_e1m7, &map_e1m6, &map_e1m5, &map_e1m2, &map_e1m3, &map_e1m4,
    &map_e1m1, &map_e2m2, &map_e2m3, &map_e2m4, &map_e2m5, &map_e2m6, &map_e2m7, &map_e2m8, &map_e2m9,
    &map_e3m1, &map_e3m2, &map_e3m3, &map_e3m4, &map_e3m5, &map_e3m6, &map_e3m7, &map_e3m8, &map_e3m9
};
/******************************************************************************/

