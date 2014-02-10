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

#ifndef __Y3D_YETI3D_H__
#define __Y3D_YETI3D_H__

/******************************************************************************/

#include "y3d_viewport.h"

#include "y3d_ansic.h"
#include "y3d_matrix.h"
#include "y3d_fixed.h"
#include "y3d_vertex.h"
#include "y3d_texture.h"
#include "y3d_surface.h"
#include "y3d_keyboard.h"
#include "y3d_map.h"
#include "y3d_pixel.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

#define YETI_STR_TITLE      "Yeti3D Engine"
#define YETI_STR_VERSION    "PRO"
#define YETI_STR_COPYRIGHT  "Copyright (C) 2003-04 Derek J. Evans. All Rights Reserved"
#define YETI_STR_URL        "http://www.theteahouse.com.au/gba/"
#define YETI_STR_CAPTION    (YETI_STR_TITLE " " YETI_STR_VERSION " - " YETI_STR_COPYRIGHT)

/******************************************************************************/

#define YETI_SKIN_WIDTH 128
#define YETI_SKIN_HEIGHT 128
#define YETI_BULLET_MAX 8
#define YETI_BULLET_MASK (YETI_BULLET_MAX - 1)

/******************************************************************************/

#ifdef __WIN32__
#define YETI_RAY_WIDTH 100
#define YETI_RAY_MAX 256
#ifdef __YETI_EDITOR__
#define YETI_CELL_MAX 4096
#else
#define YETI_CELL_MAX 1024
#endif
#else
#define YETI_RAY_WIDTH 350
#define YETI_RAY_MAX 64
#define YETI_CELL_MAX 512
#endif

/******************************************************************************/

#define YETI_MODEL_BUCKET_MAX 16
#define YETI_MODEL_TRANGLE_MAX 1000
#define YETI_MODEL_VERTEX_MAX 500

/******************************************************************************/

typedef struct fontchar_t {
    u8 pixels[8][8];
} fontchar_t;

typedef u16 skin_t[YETI_SKIN_HEIGHT][YETI_SKIN_WIDTH];

typedef u16 lut_t[256];
typedef lut_t lua_t[64];
typedef lua_t lighting_t[4];
typedef u8 color_t[3];
typedef color_t palette_t[256];

/******************************************************************************/

typedef struct point8_t {
    u8 x, y;
} point8_t;

typedef struct ray_t {
    int x, xx;
    int y, yy;
} ray_t;

/******************************************************************************/

typedef struct menu_t {
    s8 main;
    s8 skill;
    s8 episode;
    s8 paused;
    s8 option;
    s8 language;
} menu_t;

typedef struct message_t {
    string_t s;
    unsigned tick;
    char *text;
} message_t;

typedef struct hud_t {
    u8 health;
    u8 ammo;
} hud_t;

typedef struct game_t {
    s16 gravity;    // Gravity force.
    s16 jumping;    // Jumping force.
    s16 crawling;   // Crawling height.
    s16 walking;    // Walking height.
    s16 ceiling;    // Crouch distance.
    s16 tick;
    hud_t hud;
    menu_t menu;
    s8  prevmode;
    s16  episode;
} game_t;

typedef struct visual_t {
    int threshold_dithering;
    int threshold_perspective;
    int map_lod;
    int brightness;
} visual_t;

typedef STATICARRAY(point8_t, YETI_CELL_MAX) cell_vis_t;
typedef STATICARRAY(entity_t *, YETI_BULLET_MAX) bullet_array_t;

typedef struct yeti_t {
    game_t game;

    u32 mark;
    u32 fps;
    entity_t *camera;
    entity_t *player;
    entity_t *player2;

    visual_t    visual;
    viewport_t  viewport;
    keyboard_t  keyboard;
    message_t   message;
    map_t       map;

    int is2d;
    matrix_t m;

    entity_array_t  entities;
    vertex_pool_t   vpool;
    surface_array_t surfaces;
    bullet_array_t  bullets;

    texture_t  *textures;
    color_t    *palette;
    lua_t       luas[4];
    u16        *skyimage;

    vertex_t    verts[YETI_MODEL_VERTEX_MAX];

    void (*onhit)(struct yeti_t *yeti, struct entity_t *e1, struct entity_t *e2);
} yeti_t;

#define YETI_BULLET(YETI) ((YETI)->bullets.item[(YETI)->bullets.length++ & YETI_BULLET_MASK])

/******************************************************************************/

extern void yeti_load_map(yeti_t *yeti, rom_map_t *map);
extern void yeti_save_map(yeti_t *yeti, rom_map_t *map);
extern void yeti_init(yeti_t *yeti,
                      u16 *front,
                      u16 *back,
                      texture_t *textures,
                      palette_t palette);

extern entity_t *yeti_entity(yeti_t *yeti, int x, int y, int z);

extern void yeti_save_cell(cell_t *src, rom_cell_t *dst);
extern void yeti_init_cell(yeti_t *yeti, int x, int y);

extern void yeti_init_lua(yeti_t *yeti, float ga);

extern void message_init(message_t *message, char *text);

/******************************************************************************/

extern texture_t textures[TEXTURE_MAX];
extern palette_t palette;

/******************************************************************************/

#ifdef __cplusplus
} // extern "C"
#endif

/******************************************************************************/

#endif


