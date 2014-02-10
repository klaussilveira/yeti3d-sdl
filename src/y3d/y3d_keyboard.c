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

#include "y3d_keyboard.h"

/******************************************************************************/

/*
** Name: keyboard_init
** Desc:
*/
void keyboard_init(keyboard_t *keyboard)
{
    CLEARMEM(keyboard);
    //memset(&keyboard->state, -1, sizeof(keyboard->state));
    //memset(&keyboard->saved, -1, sizeof(keyboard->saved));
}

/*
** Name: keyboard_tick
** Desc: Takes a copy of the current keyboard flags. Used to detect
**       keypresses.
*/
void keyboard_tick(keyboard_t *keyboard)
{
    keyboard->tick++;
    keyboard->saved = keyboard->state;
}
