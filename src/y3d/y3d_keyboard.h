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

#ifndef __Y3D_KEYBOARD_H__
#define __Y3D_KEYBOARD_H__

#include "y3d_ansic.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

// Keyboard structure. The keys are based on the GBA keyboard set. Previous
// frame settings are stored so you can test for keydown and keyup events
// by checking a keys previous setting.
// eg: selectdown = kb->select & !kb->_select;

// NOTE: The first 8 bits are used for multiplayer & recording features so
// they must not be changed.

typedef struct keys_t {
    // Multiple player keys. Must be 8 bits.
    u16     left: 1;
    u16     right: 1;
    u16     up: 1;
    u16     down: 1;
    u16     a: 1;
    u16     b: 1;
    u16     r: 1;
    u16     l: 1;

    // Extra non-mp keys. More can be added if required.
    u16     select: 1;
    u16     start: 1;
    u16     rotate: 1;
    point_t mouse; // Not supported on all platforms.
} keys_t;

typedef struct keyboard_t {
    unsigned  tick;
    keys_t    state;
    keys_t    saved;
} keyboard_t;

/******************************************************************************/

// Usefull keyboard macros.

#define KEYPRESS_A(A) ((A)->keyboard.state.a && !(A)->keyboard.saved.a)
#define KEYPRESS_B(A) ((A)->keyboard.state.b && !(A)->keyboard.saved.b)
#define KEYPRESS_UP(A) ((A)->keyboard.state.up && !(A)->keyboard.saved.up)
#define KEYPRESS_DN(A) ((A)->keyboard.state.down && !(A)->keyboard.saved.down)
#define KEYPRESS_SELECT(A) ((A)->keyboard.state.select && !(A)->keyboard.saved.select)

/******************************************************************************/

extern void keyboard_init(keyboard_t *keyboard);
extern void keyboard_tick(keyboard_t *keyboard);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif

