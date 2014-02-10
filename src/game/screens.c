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

#include "../y3d/y3d_draw.h"
#include "../y3d/y3d_engine.h"
#include "../y3d/y3d_image.h"

#include "screens.h"
#include "game.h"
#include "sprites.h"

/*
** Name: draw_menu_item
** Desc:
*/
void draw_menu_item(yeti_t *yeti, int x, int y, char *s, int xs, int ys, int selected)
{
    if (selected) {
        int y1 = TEXT_COORD(y + 0, ys);
        int y2 = TEXT_COORD(y + 1, ys);
        image_draw(
            yeti->viewport.back->pixels[0],
            YETI_VIEWPORT_X1, y1, YETI_VIEWPORT_X2, y2,
            IMAGE(spr_menu), DRAW_MODE_COPY);
    }

    draw_string(yeti->viewport.back, x, y, s, xs, ys,
                selected ? DRAW_MODE_COLORKEY : DRAW_MODE_COLORKEY_BLEND,
                128);
}

/*
** Name: screen_monster
** Desc:
*/
void screen_monster(yeti_t *yeti)
{
    int y;
    int v  = 0;
    int vv = f2i(IMAGE_HEIGHT(spr_face) * reciprocal[YETI_VIEWPORT_HEIGHT]);

    for (y = YETI_VIEWPORT_Y1; y < YETI_VIEWPORT_Y2; y++) {
        int x1 = (fixsin(((yeti->keyboard.tick << 2) + y) << 7) >> 3) - (YETI_VIEWPORT_XCENTER << 0);
        int x2 = (fixsin(((yeti->keyboard.tick << 2) + y) << 7) >> 3) + (YETI_VIEWPORT_XCENTER << 0);
        image_draw(yeti->viewport.back->pixels[0],
                   YETI_VIEWPORT_X1 + x1, y, YETI_VIEWPORT_X2 + x2, y + 1,
                   spr_face, 0, f2i(v), IMAGE_WIDTH(spr_face), f2i(v) + 1,
                   DRAW_MODE_COPY);
        v += vv;
    }
}

/*
** Name: screen_flames
** Desc:
*/
void screen_flames(yeti_t *yeti, int rising)
{
    u16 *flames[] = {spr_flames0, spr_flames1, spr_flames2, spr_flames3};
    u16 *flame = flames[(yeti->game.tick >> 2) & 3];
    screen_monster(yeti);

    if (rising) {
        int zoom = fixfrac(yeti->game.tick);
        image_draw(
            yeti->viewport.back->pixels[0],
            0, FIXLERP(YETI_VIEWPORT_Y2, YETI_VIEWPORT_YCENTER, zoom),
            YETI_VIEWPORT_X2, YETI_VIEWPORT_Y2,
            IMAGE(flame), DRAW_MODE_COLORKEY_BLEND);
    } else {
        image_draw(
            yeti->viewport.back->pixels[0],
            YETI_VIEWPORT_X1, YETI_VIEWPORT_YCENTER,
            YETI_VIEWPORT_X2, YETI_VIEWPORT_Y2,
            IMAGE(flame), DRAW_MODE_COLORKEY_BLEND);
    }
}

/*
** Name: screen_yeti3d
** Desc:
*/
void screen_yeti3d(yeti_t *yeti)
{
    int zoom = fixfrac(yeti->game.tick) * 4 - 512;
    zoom = MAX(zoom, 0);
    screen_flames(yeti, 1);
    image_draw(
        yeti->viewport.back->pixels[0],
        YETI_VIEWPORT_X1,
        FIXLERP(YETI_VIEWPORT_Y1, YETI_VIEWPORT_YCENTER, zoom),
        YETI_VIEWPORT_X2,
        FIXLERP(YETI_VIEWPORT_Y2, YETI_VIEWPORT_YCENTER, zoom),
        IMAGE(spr_yeti3d), DRAW_MODE_COLORKEY);
}

/*
** Name: screen_publisher
** Desc:
*/
void screen_publisher(yeti_t *yeti)
{
    int zoom2 = fixfrac(yeti->game.tick) * 4;
    zoom2 = MIN(zoom2, i2f(1));
    image_draw(
        yeti->viewport.back->pixels[0],
        FIXLERP(YETI_VIEWPORT_XCENTER, YETI_VIEWPORT_X1, zoom2),
        FIXLERP(YETI_VIEWPORT_YCENTER, YETI_VIEWPORT_Y1, zoom2),
        FIXLERP(YETI_VIEWPORT_XCENTER, YETI_VIEWPORT_X2, zoom2),
        FIXLERP(YETI_VIEWPORT_YCENTER, YETI_VIEWPORT_Y2, zoom2),
        IMAGE(spr_publisher), DRAW_MODE_COPY);
}

/*
** Name: screen_title
** Desc:
*/
void screen_title(yeti_t *yeti)
{
    int zoom1 = fixfrac(yeti->game.tick) * 4;
    int zoom2 = MIN(zoom1, 256);
    screen_flames(yeti, 0);
    image_draw(
        yeti->viewport.back->pixels[0],
        FIXLERP(YETI_VIEWPORT_XCENTER, YETI_VIEWPORT_X1, zoom2),
        FIXLERP(YETI_VIEWPORT_YCENTER, YETI_VIEWPORT_Y1, zoom2),
        FIXLERP(YETI_VIEWPORT_XCENTER, YETI_VIEWPORT_X2, zoom2),
        FIXLERP(YETI_VIEWPORT_YCENTER, YETI_VIEWPORT_Y2, zoom2),
        IMAGE(spr_temple),
        zoom1 < 200 ? DRAW_MODE_COLORKEY_BLEND : DRAW_MODE_COLORKEY);
}

/*
** Name: screen_menu
** Desc:
*/
void screen_menu(yeti_t *yeti)
{
    u16 *animation[] = {spr_blood0, spr_blood1, spr_blood2, spr_blood3};
    u16 *frame = animation[(yeti->game.tick >> 3) & 3];
    screen_flames(yeti, 0);

    if (yeti->game.menu.main < 0) {
        yeti->game.menu.main = 0;
    }

    if (yeti->game.menu.main > 3) {
        yeti->game.menu.main = 3;
    }

#ifdef __GBA__
    image_draw(
        yeti->viewport.back->pixels[0],
        0, 0, YETI_VIEWPORT_WIDTH, 16,
        IMAGE(frame), DRAW_MODE_COLORKEY_BLEND);
    draw_string(yeti->viewport.back, 2, 0, "The  Temple", 3, 4, DRAW_MODE_COLORKEY, 0);
    draw_menu_item(yeti, 3, 3, "New  Game"    , 3, 3, yeti->game.menu.main == 0);
    draw_menu_item(yeti, 4, 4, "Options"      , 3, 3, yeti->game.menu.main == 1);
    draw_menu_item(yeti, 3, 5, "Load Game"    , 3, 3, yeti->game.menu.main == 2);
    draw_menu_item(yeti, 4, 6, "Credits"      , 3, 3, yeti->game.menu.main == 3);
#else
    image_draw(
        yeti->viewport.back->pixels[0],
        0, 0, YETI_VIEWPORT_WIDTH, 48,
        IMAGE(frame), DRAW_MODE_COLORKEY_BLEND);
    draw_string(yeti->viewport.back, 0, 1, "The Temple", 5, 4, DRAW_MODE_COLORKEY, 0);
    draw_menu_item(yeti, 6, 2, "New Game"     , 4, 5, yeti->game.menu.main == 0);
    draw_menu_item(yeti, 6, 3, "Options"      , 4, 5, yeti->game.menu.main == 1);
    draw_menu_item(yeti, 6, 4, "Load Game"    , 4, 5, yeti->game.menu.main == 2);
    draw_menu_item(yeti, 6, 5, "Credits"      , 4, 5, yeti->game.menu.main == 3);
#endif
}

/*
** Name: screen_episode
** Desc:
*/
void screen_episode(yeti_t *yeti)
{
    screen_flames(yeti, 0);
    yeti->game.menu.episode = CLAMP(yeti->game.menu.episode, 0, 2);
#ifdef __GBA__
    draw_string(yeti->viewport.back, 0, 0, "Select  Episode", 3, 4, DRAW_MODE_COLORKEY, 0);
    draw_menu_item(yeti, 3, 3, "Evil Dead"      , 3, 3, yeti->game.menu.episode == 0);
    draw_menu_item(yeti, 0, 4, "Secret  Caverns", 3, 3, yeti->game.menu.episode == 1);
    draw_menu_item(yeti, 2, 5, "The Temple"     , 3, 3, yeti->game.menu.episode == 2);
#else
    draw_string(yeti->viewport.back, 3, 1, "Select Episode", 4, 5, DRAW_MODE_COLORKEY, 0);
    draw_menu_item(yeti, 5, 3, "Evil Dead"      , 4, 5, yeti->game.menu.episode == 0);
    draw_menu_item(yeti, 3, 4, "Secret Caverns" , 4, 5, yeti->game.menu.episode == 1);
    draw_menu_item(yeti, 5, 5, "The Temple"     , 4, 5, yeti->game.menu.episode == 2);
#endif
}

/*
** Name: screen_skill
** Desc:
*/
void screen_skill(yeti_t *yeti)
{
    screen_flames(yeti, 0);
    yeti->game.menu.skill = CLAMP(yeti->game.menu.skill, 0, 2);
#ifdef __GBA__
    draw_string(yeti->viewport.back, 2, 0, "Skill Level", 3, 4, DRAW_MODE_COLORKEY, 0);
    draw_menu_item(yeti, 5, 3, "Easy"       , 3, 3, yeti->game.menu.skill == 0);
    draw_menu_item(yeti, 4, 4, "Normal"     , 3, 3, yeti->game.menu.skill == 1);
    draw_menu_item(yeti, 2, 5, "Nightmare!" , 3, 3, yeti->game.menu.skill == 2);
#else
    draw_string(yeti->viewport.back, 4, 1, "Choose Skill", 4, 5, DRAW_MODE_COLORKEY, 0);
    draw_menu_item(yeti, 8, 3, "Easy"       , 4, 5, yeti->game.menu.skill == 0);
    draw_menu_item(yeti, 7, 4, "Normal"     , 4, 5, yeti->game.menu.skill == 1);
    draw_menu_item(yeti, 5, 5, "Nightmare!" , 4, 5, yeti->game.menu.skill == 2);
#endif
}

/*
** Name: screen_credits
** Desc:
*/
void screen_credits(yeti_t *yeti)
{
#ifdef __GBA__
    int x = 3, y = 3, l = 0;
#else
    int x = 4, y = 4, l = 0;
#endif
    screen_flames(yeti, 0);
    draw_string(yeti->viewport.back, 0, l++, "Developer:"       , x, y, DRAW_MODE_COLORKEY, 0);
    draw_string(yeti->viewport.back, 1, l++, "Derek Evans"      , x, y, DRAW_MODE_COLORKEY, 128);
    draw_string(yeti->viewport.back, 1, l++, "AlienKinetics"    , x, y, DRAW_MODE_COLORKEY, 128);
    l++;
    draw_string(yeti->viewport.back, 0, l++, "Special Thanks:"  , x, y, DRAW_MODE_COLORKEY, 0);
    draw_string(yeti->viewport.back, 1, l++, "Greg Torlus"      , x, y, DRAW_MODE_COLORKEY, 128);
    draw_string(yeti->viewport.back, 1, l++, "Matt ThunderZ"    , x, y, DRAW_MODE_COLORKEY, 128);
    draw_string(yeti->viewport.back, 1, l++, "Phil Harvey"      , x, y, DRAW_MODE_COLORKEY, 128);
    draw_string(yeti->viewport.back, 1, l++, "Sean Cross"       , x, y, DRAW_MODE_COLORKEY, 128);
    draw_string(yeti->viewport.back, 1, l++, "Aardappel"        , x, y, DRAW_MODE_COLORKEY, 128);
}

void draw_option(yeti_t *yeti, int x, int y, char *s, int xs, int ys, int selected, int value)
{
    char tmp[32];
    char *p = ansic_stpcpy(ansic_stpcpy(tmp, s), ": ");

    if (value ==  0) {
        strcpy(p, "Off");
    } else if (value == -1) {
        strcpy(p, "On");
    } else {
        ansic_itos(value, p);
    }

    draw_menu_item(yeti, x, y, tmp, xs, ys, selected);
}

/*
** Name: screen_view_options
** Desc:
*/
void screen_view_options(yeti_t *yeti)
{
#ifdef __GBA__
    int x = 3, y = 3, l = 0;
#else
    int x = 4, y = 4, l = 0;
#endif
    yeti->game.menu.option = CLAMP(yeti->game.menu.option, 0, 5);
    screen_flames(yeti, 0);
    draw_option(yeti, 0, l++, "Messages"    , x, y, yeti->game.menu.option == 0, 0);
    draw_option(yeti, 0, l++, "SFX"         , x, y, yeti->game.menu.option == 1, 0);
    draw_option(yeti, 0, l++, "Music"       , x, y, yeti->game.menu.option == 2, 0);
    draw_option(yeti, 0, l++, "Lighting"    , x, y, yeti->game.menu.option == 3, 0);
    draw_option(yeti, 0, l++, "Brightness"  , x, y, yeti->game.menu.option == 4, yeti->visual.brightness);
    draw_menu_item(yeti, 0, l++, "Set Controls" , x, y, yeti->game.menu.option == 5);
}

/*
** Name: screen_load_game
** Desc:
*/
void screen_load_game(yeti_t *yeti)
{
    screen_flames(yeti, 0);
    draw_string(yeti->viewport.back, 1, 1, "Load Not", 3, 3, DRAW_MODE_COLORKEY, 0);
    draw_string(yeti->viewport.back, 1, 2, "Avaliable", 3, 3, DRAW_MODE_COLORKEY, 0);
}

/*
** Name: screen_quit_game
** Desc:
*/
void screen_quit_game(yeti_t *yeti)
{
    screen_flames(yeti, 0);
    draw_string(yeti->viewport.back, 1, 1, "Quit Not", 3, 3, DRAW_MODE_COLORKEY, 0);
    draw_string(yeti->viewport.back, 1, 2, "Avaliable", 3, 3, DRAW_MODE_COLORKEY, 0);
}

/*
** Name: screen_play
** Desc:
*/
void screen_play(yeti_t *yeti)
{
    char s[10];
    yeti_draw(yeti);
    s[0] = 1 + ((yeti->keyboard.tick >> 3) & 1);
    ansic_itos(yeti->game.hud.health, s + 1);
#ifdef __GBA__
    draw_string(yeti->viewport.back, 1, 14, s, 3, 3, DRAW_MODE_COLORKEY, 0);
#else
    draw_string(yeti->viewport.back, 1, 13, s, 4, 4, DRAW_MODE_COLORKEY_BLEND, 0);
#endif
    s[0] = 3;
    ansic_itos(yeti->game.hud.ammo, s + 1);
#ifdef __GBA__
    draw_string(yeti->viewport.back, 15, 14, s, 3, 3, DRAW_MODE_COLORKEY, 0);
#else
    draw_string(yeti->viewport.back, 15, 13, s, 4, 4, DRAW_MODE_COLORKEY_BLEND, 0);
#endif
}

/*
** Name: screen_paused
** Desc:
*/
void screen_paused(yeti_t *yeti)
{
    yeti->game.menu.paused = CLAMP(yeti->game.menu.paused, 0, 3);
    screen_play(yeti);
#ifdef __GBA__
    draw_string(yeti->viewport.back, 2, 0, "Game Paused", 3, 4, DRAW_MODE_COLORKEY, 0);
    draw_menu_item(yeti, 2, 3, "Resume Play"  , 3, 3, yeti->game.menu.paused == 0);
    draw_menu_item(yeti, 4, 4, "Options"      , 3, 3, yeti->game.menu.paused == 1);
    draw_menu_item(yeti, 3, 5, "Load Game"    , 3, 3, yeti->game.menu.paused == 2);
    draw_menu_item(yeti, 3, 6, "Quit Game"    , 3, 3, yeti->game.menu.paused == 3);
#else
    draw_string(yeti->viewport.back, 4, 1, "Game Paused", 4, 5, DRAW_MODE_COLORKEY, 0);
    draw_menu_item(yeti, 4, 5, "Resume Play"  , 4, 4, yeti->game.menu.paused == 0);
    draw_menu_item(yeti, 6, 6, "Options"      , 4, 4, yeti->game.menu.paused == 1);
    draw_menu_item(yeti, 5, 7, "Load Game"    , 4, 4, yeti->game.menu.paused == 2);
    draw_menu_item(yeti, 5, 8, "Quit Game"    , 4, 4, yeti->game.menu.paused == 3);
#endif
}

/*
** Name: screen_end_level
** Desc:
*/
void screen_end_level(yeti_t *yeti)
{
    yeti_draw(yeti);
}

