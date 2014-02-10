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

#ifndef __Y3D_ENTITY_H__
#define __Y3D_ENTITY_H__

struct yeti_t;
struct entity_t;
struct cell_t;

/******************************************************************************/

#include "y3d_animation.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define ENTITY_CELL(A,B) (&(A)->map.item[f2i((B)->y)][f2i((B)->x)])

#define ENTITY_NONE         0
#define ENTITY_CAMERA       1
#define ENTITY_EXIT         2
#define ENTITY_SECRET       3
#define ENTITY_EXIT_CAMERA  4
#define ENTITY_LIGHT        5
#define ENTITY_BLOCKER      255

typedef struct entity_visual_t {
    u8    altpitch: 1; // Use alternate pitch rotation.
    u8    altroll: 1; // Use alternate roll rotation.
    u8    altturn: 1; // Use alternate turn rotation.
    u8    mode: 3;    // Visual rendering mode.
    s16   width;      // 8:8 width.
    s16   height;     // 8:8 height.
    void *data;       // Rendering data. ie: Sprite or model.
    void (*ondraw)(struct yeti_t *yeti, struct entity_t *e);
} entity_visual_t;

typedef struct entity_state_t {
    u16 pain;
} entity_state_t;

typedef struct entity_t {
    struct entity_t *next;
    void *tag;

    entity_visual_t visual;
    animation_t animation;

    u8    emitlight: 1;   // Entity emits light.
    u8    castshadow: 1;
    u8    nocollision: 1;
    u8    alwaysvisible: 1;
    u8    alwaysontop: 1;

    entity_state_t state;

    u16   radius;         // 8:8 entity radius.
    s16   x, xx;          // 8:8 X position and velocity.
    s16   y, yy;          // 8:8 Y position and velocity.
    s16   z, zz;          // 8:8 Z position and velocity.
    s16   p, pp;          // Pitch and pitch velocity.
    s16   r, rr;          // Roll and roll velocity.
    s16   t, tt;          // Turn and turn velocity.
    s16   rx;             // View space X position.
    s16   ry;             // View space Y position
    s16   rz;             // View space Z position

    void (*ontick)(struct yeti_t *yeti, struct entity_t *e);
    void (*onhit)(struct yeti_t *yeti, struct entity_t *e1, struct entity_t *e2);
    void (*onwall)(struct yeti_t *yeti, struct entity_t *e, struct cell_t *c);
} entity_t;

typedef void (*entity_behaviour_t)(struct yeti_t *yeti, entity_t *e);

// The maxium number of entities allocated. Instead of increasing this, try
// to reuse entities via entity pools.
#define ENTITY_MAX 70

typedef STATICARRAY(entity_t, ENTITY_MAX) entity_array_t;

/******************************************************************************/

extern void entity_destroy(entity_t *e);
extern void entity_init(entity_t *e, int x, int y, int z);
extern void entity_reinit(entity_t *e);

extern void entity_freeze(entity_t *e);
extern void entity_friction(entity_t *e, int amount);
extern void entity_move_forward(entity_t *e);
extern void entity_move_backwards(entity_t *e);
extern void entity_turn_right(entity_t *e);
extern void entity_turn_left(entity_t *e);
extern void entity_turn_towards(entity_t *e, int x, int y);
extern void entity_look_at(entity_t *e, int x, int y, int z);
extern void entity_look_at2(entity_t *e, int x, int y, int z);
extern void entity_set_velocity(entity_t *e);
extern void entity_force_towards(entity_t *e, int x, int y, int z, int shift);

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif
