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

#include "y3d_yeti.h"
#include "y3d_draw.h"
#include "y3d_image.h"
#include "y3d_map.h"
#include "y3d_engine.h"

/******************************************************************************/

/*
** Name: palette_overbright
** Desc: Converts a standard palette to a overbright palette. Use this on
**       platforms that bypass the LUA.
** Note: This isn't used anymore since I found a way to use LUA data on
**       hardware rendered platforms.
*/
void palette_overbright(palette_t dst, palette_t src, int brightness)
{
    int i;

    for (i = 0; i < 256; i++) {
        int r = src[i][0] * brightness / FIXONE;
        int g = src[i][1] * brightness / FIXONE;
        int b = src[i][2] * brightness / FIXONE;
        dst[i][0] = CLAMP(r, 0, 255);
        dst[i][1] = CLAMP(g, 0, 255);
        dst[i][2] = CLAMP(b, 0, 255);
    }
}

/******************************************************************************/

/*
** Name: yeti_entity
** Desc: Creates a new entity and returns a entity pointer.
*/
entity_t *yeti_entity(yeti_t *yeti, int x, int y, int z)
{
    entity_t *entity = &yeti->entities.item[yeti->entities.length++];
    entity_init(entity, x, y, z);
    return entity;
}

/******************************************************************************/

/*
** Name: yeti_load_cell
** Desc: Loads a cell from a ROM based structure.
*/
void yeti_load_cell(cell_t *dst, rom_cell_t *src)
{
    CLEARMEM(dst);
    dst->swi = src->swi;
    dst->ent = src->ent;
    dst->top = src->top;
    dst->bot = src->bot;
    dst->lit = src->lit;
    dst->wtt = src->wtt;
    dst->ttx = src->ttx;
    dst->btx = src->btx;
    dst->bos = src->bos;
    dst->tos = src->tos;
    dst->wwi = src->wwi;
    dst->wtb = src->wtb;
    dst->sw2 = src->sw2;

    if (!dst->wtb) {
        dst->wtb = dst->wtt;
    }

#ifndef __GBA__

    if (CELL_IS_SOLID(dst)) {
        dst->tsa = i2f(4);
        dst->bsa = i2f(0);
    } else {
        dst->tsa = 0;
        dst->bsa = 0;
    }

#endif
}

/*
** Name: yeti_save_cell
** Desc: Saves a cell to a ROM based structure.
*/
void yeti_save_cell(cell_t *src, rom_cell_t *dst)
{
    dst->swi = src->swi;
    dst->ent = src->ent;
    dst->top = src->top;
    dst->bot = src->bot;
    dst->lit = src->lit;
    dst->wtt = src->wtt;
    dst->ttx = src->ttx;
    dst->btx = src->btx;
    dst->bos = src->bos;
    dst->tos = src->tos;
    dst->wwi = src->wwi;
    dst->wtb = src->wtb;
    dst->sw2 = src->sw2;
}

/******************************************************************************/

/*
** Name: yeti_load_map
** Desc:
*/
void yeti_load_map(yeti_t *yeti, rom_map_t *src)
{
    int i, x, y;
    keyboard_init(&yeti->keyboard);

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            yeti_load_cell(&yeti->map.item[y][x], &src->cells[y][x]);
        }
    }

    map_precalc_data(&yeti->map, yeti->visual.map_lod);
    yeti->entities.length = 0;
    yeti->camera = yeti_entity(yeti, i2fdiv2(MAP_WIDTH), i2fdiv2(MAP_HEIGHT), 0);
    yeti->player = yeti->camera;
    yeti->camera->t = 0;
    yeti->camera->r = 0;
    yeti->camera->p = i2f(8);

    for (i = YETI_BULLET_MAX; i--;) {
        yeti->bullets.item[i] = yeti_entity(yeti, 0, 0, 0);
    }

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            cell_t *cell = &yeti->map.item[y][x];

            switch (cell->ent) {
            case ENTITY_CAMERA: {
                yeti->camera->x = i2f(x) + FIXHALF;
                yeti->camera->y = i2f(y) + FIXHALF;
                yeti->camera->z = cell_bot(cell, yeti->camera) + i2f(2);
                break;
            }

            case ENTITY_BLOCKER: {
                cell->eos = i2f(16);
                break;
            }
            }
        }
    }
}

/*
** Name: yeti_save_map
** Desc: Saves the current map to a rom based map.
*/
void yeti_save_map(yeti_t *yeti, rom_map_t *map)
{
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            yeti_save_cell(&yeti->map.item[y][x], &map->cells[y][x]);
        }
    }
}

/******************************************************************************/

/*
** Name: visual_init
** Desc:
*/
void visual_init(visual_t *vq)
{
    CLEARMEM(vq);
    vq->threshold_dithering   = i2f(3);
    vq->threshold_perspective = i2f(6);
    vq->brightness = 1;
    vq->map_lod = 4;
#ifdef __YETI_EDITOR__
    vq->map_lod = 1;
#endif
#ifdef __POCKETPC__
    vq->threshold_dithering   = i2f(1);
    vq->threshold_perspective = i2f(2);
#endif
#ifdef __GBA__
    vq->map_lod = 6;
    vq->brightness = 2;
#endif
}

/*
** Name: yeti_init
** Desc: Setup a default map and position the camera.
*/
void yeti_init(
    yeti_t *yeti,
    u16 *front,
    u16 *back,
    texture_t *textures,
    palette_t palette)
{
    int i;
    CLEARMEM(yeti);

    for (i = 0; i < YETI_RECIPROCAL_MAX; i++) {
        reciprocal[i] = i ? (1 << 16) / i : 0;
    }

    for (i = 0; i < YETI_SINTABLE_MAX; i++) {
        sintable[i] = fl2f(sin(i * M_PI / 1024.0f));
    }

    yeti->viewport.front  = (pixel_buffer_t *) front;
    yeti->viewport.back   = (pixel_buffer_t *) back;
    yeti->textures        = textures;
    yeti->palette         = palette;
    visual_init(&yeti->visual);
    keyboard_init(&yeti->keyboard);
    surface_array_init(&yeti->surfaces);
    map_init(&yeti->map, yeti->visual.map_lod);
}

/******************************************************************************/

/*
** Name: message_init
** Desc: Setup a time limited message. Used for in game message displays.
*/
void message_init(message_t *message, char *text)
{
    message->text = text;
    message->tick = 64;
}

/******************************************************************************/

/*
** Name: lua_init
** Desc: Initilize a colour look up array.
*/
void lua_init(lua_t lua, palette_t palette, float rs, float gs, float bs, float ga)
{
    int x, y;
    lut_t lut;

    for (x = 256; x--;) {
        lut[x] = pow(x / 255.0f, 1.0f / ga) * 31.0f;
    }

    for (x = 256; x--;) {
        u8 *rgb = palette[x];

        for (y = 64; y--;) {
            int z = MAX(y - 2 , 0);
            int r = rgb[0] * rs * z / 32.0f;
            int g = rgb[1] * gs * z / 32.0f;
            int b = rgb[2] * bs * z / 32.0f;
            lua[y][x] = RGB_SET(
                            lut[CLAMP(r, 0, 255)],
                            lut[CLAMP(g, 0, 255)],
                            lut[CLAMP(b, 0, 255)]);
        }
    }
}

/*
** Name: yeti_init_lua
** Desc: Initilize the four yeti lighting arrays.
*/
void yeti_init_lua(yeti_t *yeti, float ga)
{
    lua_init(yeti->luas[0], yeti->palette, 0.7f, 0.6f, 0.6f, ga);
    //lua_init(yeti->luas[0], yeti->palette, 0.7f, 0.7f, 0.7f, ga);
    lua_init(yeti->luas[1], yeti->palette, 0.7f, 0.3f, 0.3f, ga);
    lua_init(yeti->luas[2], yeti->palette, 0.3f, 0.7f, 0.3f, ga);
    lua_init(yeti->luas[3], yeti->palette, 0.3f, 0.3f, 0.7f, ga);
}




