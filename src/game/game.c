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

#include "../y3d/y3d_ansic.h"
#include "../y3d/y3d_sound.h"
#include "../y3d/y3d_image.h"
#include "../y3d/y3d_draw.h"
#include "../y3d/y3d_quake.h"
#include "../y3d/y3d_engine.h"

#include "game.h"
#include "maps.h"
#include "models.h"
#include "sprites.h"
#include "entities.h"
#include "screens.h"

/******************************************************************************/

void entity_default(yeti_t *yeti, entity_t *e, int isjumping, int iscrawling, int xyfriction)
{
    cell_t *cell = ENTITY_CELL(yeti, e);
    int bot = cell_bot(cell, e) + (iscrawling ? yeti->game.crawling : yeti->game.walking);
    int top = cell_top(cell, e) - yeti->game.ceiling;

    //if (TEXTURE_IS_LIQUID(cell->btx)) bot -= 32;

    if (e == yeti->player) {
        e->r = ansic_friction(e->r, 64);
        e->p = ansic_friction(e->p, 30);
    }

    if (e->z <= bot) {
        e->z  -= (e->z - bot) >> 2;
        e->zz -= e->zz >> 2;

        if (isjumping && e->z < top) {
            e->z = bot;
            e->zz += yeti->game.jumping;
        }

        if (TEXTURE_IS_LIQUID(cell->btx)) {
            //xyfriction += 20;
        }
    } else {
        e->zz += yeti->game.gravity;
    }

    if (e->z >= top) {
        e->z -= (e->z - top) >> 2;
    }

    e->xx = ansic_friction(e->xx,  xyfriction);
    e->yy = ansic_friction(e->yy,  xyfriction);
    e->tt = ansic_friction(e->tt, 110);

    if (e->state.pain > i2f(1)) {
        death_init(yeti, e);
    }
}

/******************************************************************************/

void camera_exit_tick(yeti_t *yeti, entity_t *e)
{
    cell_t *cell = ENTITY_CELL(yeti, e);
    e->z = cell_bot(cell, e) + i2f(2);
    entity_look_at2(e, yeti->player->x, yeti->player->y, yeti->player->z);
}

void camera_exit_init(entity_t *e)
{
    e->ontick = camera_exit_tick;
}

/******************************************************************************/

void player_tick(yeti_t *yeti, entity_t *e)
{
    int i, x, y;
    cell_t *cell = ENTITY_CELL(yeti, e);
#ifndef __YETI_EDITOR__

    switch (cell->ent) {
    case ENTITY_SECRET: {
        message_init(&yeti->message, "You found a secret");
        break;
    }

    case ENTITY_EXIT: {
        cell->ent = 0;
        message_init(&yeti->message, "Level Completed");

        if (map_find_ent(&yeti->map, ENTITY_EXIT_CAMERA, &x, &y)) {
            entity_t *e = yeti_entity(yeti, i2f(x) + FIXHALF, i2f(y) + FIXHALF, 0);
            camera_exit_init(e);
            yeti->camera = e;
            game_goto(&yeti->game, GAME_MODE_ENDLEVEL);
        }

        break;
    }
    }

#endif

    if (yeti->keyboard.state.rotate) {
        i = yeti->keyboard.state.mouse.x << 7;
        //yeti->camera->x = (fixcos(yeti->keyboard.tick << 7) * 28) + i2f(32);
        //yeti->camera->y = (fixsin(yeti->keyboard.tick << 7) * 28) + i2f(32);
        //yeti->camera->z = i2f(2);
        entity_force_towards(e,
                             yeti->player2->x + fixcos(yeti->keyboard.tick << 8) * 3,
                             yeti->player2->y + fixsin(yeti->keyboard.tick << 8) * 2,
                             yeti->player2->z + (fixcos(yeti->keyboard.tick << 8) >> 0), 4);
        entity_look_at2(e,
                        yeti->player2->x,
                        yeti->player2->y,
                        yeti->player2->z);
        e->r = fixcos(yeti->keyboard.tick << 8) << 5;
    } else {
        animation_tick(&e->animation);

        if (e->animation.mode == 1 && !e->animation.nloops) {
            animation_init(&e->animation, 0, 0, 8, 192, LOOP_FOREVER);
        }

        e->visual.data      = NULL;// yeti->camera == yeti->player ? model_rocket : NULL;
        e->visual.altpitch  = TRUE;
        e->visual.ondraw    = yeti_model_draw;

        if (yeti->keyboard.state.l) {
            e->p -= i2f(2);
        }

        if (yeti->keyboard.state.r) {
            e->p += i2f(2);
        }

        if (yeti->keyboard.state.left) {
            entity_turn_left(e);
        }

        if (yeti->keyboard.state.right) {
            entity_turn_right(e);
        }

        if (yeti->keyboard.state.up) {
            entity_move_forward(e);
        }

        if (yeti->keyboard.state.down) {
            entity_move_backwards(e);
        }

        if (KEYPRESS_A(yeti) && yeti->game.hud.ammo > 0) {
            yeti->game.hud.ammo--;
            bullet_shoot(yeti, e);
            sound_play(SOUND_ROCKETLAUNCH);
            animation_init(&e->animation, 1, 8, 17, 192, 1);
        }

        i = KEYPRESS_B(yeti);

        if (i) {
            sound_play(SOUND_JUMP);
        }

        entity_default(yeti, e, i, FALSE, 40);
    }
}

void player_init(entity_t *e)
{
    e->radius = 0;
    e->ontick = player_tick;
    animation_init(&e->animation, 0, 0, 8, 192, LOOP_FOREVER);
}

/******************************************************************************/

extern void guard_attack_init(entity_t *e);

void guard_base_init(entity_t *e)
{
    e->visual.ondraw = yeti_model_draw;
    e->radius = 128;
}

void guard_pain_tick(yeti_t *yeti, entity_t *e)
{
    animation_tick(&e->animation);
    entity_default(yeti, e, FALSE, FALSE, 60);

    if (!e->animation.nloops) {
        guard_attack_init(e);
    }
}

void guard_pain_init(yeti_t *yeti, entity_t *e)
{
    guard_base_init(e);
    animation_mode(&e->animation, MD2_ANIM_PAIN1 + (yeti->keyboard.tick & 3), 100, 1);
    e->ontick = guard_pain_tick;
    e->onhit = NULL;
}

void guard_attack_hit(yeti_t *yeti, entity_t *e1, entity_t *e2)
{
    if (e2->ontick == bullet_tick) {
        e1->state.pain += 80;
        splat_init(yeti, e2);
        guard_pain_init(yeti, e1);
    }
}

void guard_attack_tick(yeti_t *yeti, entity_t *e)
{
    animation_tick(&e->animation);
    entity_turn_towards(e, yeti->player->x, yeti->player->y);
    entity_move_forward(e);
    entity_default(yeti, e, FALSE, FALSE, 60);
}

void guard_attack_init(entity_t *e)
{
    guard_base_init(e);
    e->ontick = guard_attack_tick;
    e->onhit  = guard_attack_hit;
    animation_mode(&e->animation, MD2_ANIM_RUN, 80, LOOP_FOREVER);
}

int entity_line_of_sight(entity_t *e1, entity_t *e2, map_t *map)
{
    return map_line_of_sight(map, e1->x, e1->y, e2->x, e2->y);
}

void guard_stand_tick(yeti_t *yeti, entity_t *e)
{
    animation_tick(&e->animation);

    if (e->animation.mode != MD2_ANIM_RUN) {
        animation_mode(&e->animation, MD2_ANIM_RUN, 80, LOOP_FOREVER);
    }

    entity_default(yeti, e, FALSE, FALSE, 60);
    {
        int i = yeti->keyboard.tick << 6;
        entity_turn_towards(e,
                            (fixcos(i) * 28) + i2f(32),
                            (fixsin(i) * 28) + i2f(32));
        entity_force_towards(e,
                             (fixcos(i) * 28) + i2f(32),
                             (fixsin(i) * 28) + i2f(32), i2f(-5), 5);
        //entity_move_forward(e);
    }
    /*
    if (entity_line_of_sight(e, yeti->player, &yeti->map))
    {
    #ifndef __YETI_EDITOR__
      guard_attack_init(e);
    #endif
    }
    */
}

void guard_stand_init(entity_t *e)
{
    guard_base_init(e);
    e->ontick = guard_stand_tick;
    e->onhit  = guard_attack_hit;
    animation_mode(&e->animation, MD2_ANIM_RUN, 80, LOOP_FOREVER);
}

void guard_init(entity_t *e)
{
    guard_stand_init(e);
}

/******************************************************************************/

/*
void tigger_init(entity_t* e)
{
  e->visual.data = model_tigger;
  guard_init(e);
}

void chicken_init(entity_t* e)
{
  e->visual.data = model_chicken;
  guard_init(e);
}

void rabbit_init(entity_t* e)
{
  e->visual.data = model_prabbit;
  guard_init(e);
}

void potator_init(entity_t* e)
{
  e->visual.data = model_potator;
  guard_init(e);
}

void karrot_init(entity_t* e)
{
  e->visual.data = model_karrot;
  guard_init(e);
}

void penguin_init(entity_t* e)
{
  e->visual.data = model_penguin;
  guard_init(e);
}

void scarlet_init(entity_t* e)
{
  e->visual.data = model_scarlet;
  guard_init(e);
}

void banana_init(entity_t* e)
{
  e->visual.data = model_banana;
  guard_init(e);
}

void goblin_init(entity_t* e)
{
  e->visual.data = model_goblin;
  guard_init(e);
}

void pm_init(entity_t* e)
{
  e->visual.data = model_pm;
  guard_init(e);
}
*/

void bauul_init(entity_t *e)
{
    e->visual.data = model_ogro;//model_bauul;
    guard_init(e);
}

void ogro_init(entity_t *e)
{
    e->visual.data = model_ogro;
    guard_init(e);
}

void rat_init(entity_t *e)
{
    e->visual.data = model_rat;
    guard_init(e);
}

/******************************************************************************/

void light_tick(yeti_t *yeti, entity_t *e)
{
    int xx = yeti->camera->x - e->x;
    int yy = yeti->camera->y - e->y;
    xx = (SQR(xx) + SQR(yy)) >> 11;
    e->visual.width = e->visual.height = CLAMP(xx, i2f(3), i2f(8));
}

void light_init(yeti_t *yeti, entity_t *e)
{
    cell_t *cell = ENTITY_CELL(yeti, e);
    //e->alwaysontop = TRUE;
    e->z = cell_top(cell, e);
    e->visual.data = spr_flare3;
    e->visual.mode = DRAW_MODE_COLORKEY_BLEND;
    e->ontick = light_tick;
}

/******************************************************************************/

/*
** Name: game_spawn_entities
** Desc: This is the function that you need to modify to create new entities.
**       You iterate the entire map and create/init entities for each map cell.
**       Each init function should then setup the default entity settings.
*/
void game_spawn_entities(yeti_t *yeti)
{
    //#ifndef __YETI_EDITOR__
    int x, y;
    yeti->entities.length = 1;

    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            int ent = yeti->map.item[y][x].ent;
            int xx = i2f(x) + FIXHALF;
            int yy = i2f(y) + FIXHALF;

            switch (ent) {
            case ENTITY_EXIT  :
                teleporter_init(yeti, yeti_entity(yeti, xx, yy, 0));
                break;

            case ENTITY_LIGHT :
                light_init(yeti, yeti_entity(yeti, xx, yy, 0));
                break;
            }

            //if (ent ==  4) rabbit_init (yeti_entity(yeti, xx, yy, 0));
            //if (ent ==  5) potator_init(yeti_entity(yeti, xx, yy, 0));
            //if (ent ==  6) karrot_init (yeti_entity(yeti, xx, yy, 0));
            //if (ent ==  7) penguin_init(yeti_entity(yeti, xx, yy, 0));
            //if (ent ==  8) scarlet_init(yeti_entity(yeti, xx, yy, 0));
            //if (ent ==  9) banana_init (yeti_entity(yeti, xx, yy, 0));
            if (ent == 10) {
                yeti->player2 = yeti_entity(yeti, xx, yy, 0);
                bauul_init(yeti->player2);
            }

            if (ent == 11) {
                ogro_init(yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 12) {
                rat_init(yeti_entity(yeti, xx, yy, 0));
            }

            //if (ent == 13) pm_init (yeti_entity(yeti, xx, yy, 0));

            if (ent == 20) {
                boost_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 21) {
                quad_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            //if (ent == 30) jeep_init   (yeti_entity(yeti, xx, yy, 0));

            if (ent == 100) {
                torch0_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 101) {
                torch1_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 102) {
                sheba_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 103) {
                leafyplant_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 104) {
                barrontree_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 105) {
                cactus_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 106) {
                potplant_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 150) {
                pinechair_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 151) {
                pillar_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 152) {
                emptypot_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            //if (ent == 153) overpass_init   (yeti, yeti_entity(yeti, xx, yy, 0));
            if (ent == 154) {
                crate_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }

            if (ent == 155) {
                bridge_init(yeti, yeti_entity(yeti, xx, yy, 0));
            }
        }
    }

    //#endif
}

void hud_init(hud_t *hud)
{
    hud->health = 100;
    hud->ammo = 150;
}

void game_load_map(yeti_t *yeti, rom_map_t *map)
{
    yeti->game.gravity  = -16;    // Gravity force.
    yeti->game.jumping  = 80;     // Jumping force.
    yeti->game.crawling = i2f(1); // Crawling height.
    yeti->game.walking  = 384;    // Walking height.
    yeti->game.ceiling  = 200;    // Crouch distance.
    yeti->skyimage = spr_sky2;
    yeti_load_map(yeti, map);
    player_init(yeti->player);
    game_spawn_entities(yeti);
    hud_init(&yeti->game.hud);
    // TODO: This message should come from the map data.
    sprintf(yeti->message.s, "Game On %d", yeti->game.episode + 1);
    message_init(&yeti->message, yeti->message.s);
}

/******************************************************************************/

int game_mode(game_t *game)
{
    return f2i(game->tick);
}

void game_goto(game_t *game, int mode)
{
    game->prevmode = game_mode(game);
    game->tick = i2f(mode);
    sound_play(SOUND_ITEMPICK);
}

void game_goback(game_t *game)
{
    game_goto(game, game->prevmode);
}

void game_cycle(game_t *game, int speed)
{
    game->tick = fixtrunc(game->tick) | fixfrac(game->tick + speed);
}

void game_menu(yeti_t *yeti, s8 *index)
{
    if (index) {
        if (KEYPRESS_UP(yeti)) {
            (*index)--;
            sound_play(SOUND_TICK);
        }

        if (KEYPRESS_DN(yeti)) {
            (*index)++;
            sound_play(SOUND_TICK);
        }
    }
}

/******************************************************************************/

void game_init(yeti_t *yeti)
{
    yeti->game.episode = 0;
    // TODO: Change this to the correct start mode.
    yeti->game.tick = i2f(GAME_MODE_TITLE);
}

/******************************************************************************/

void game_draw(yeti_t *yeti)
{
    switch (game_mode(&yeti->game)) {
    case GAME_MODE_PLAY       :
        screen_play(yeti);
        break;

    case GAME_MODE_PUBLISHER  :
        screen_publisher(yeti);
        break;

    case GAME_MODE_YETI3D     :
        screen_yeti3d(yeti);
        break;

    case GAME_MODE_TITLE      :
        screen_title(yeti);
        break;

    case GAME_MODE_MENU       :
        screen_menu(yeti);
        break;

    case GAME_MODE_PAUSED     :
        screen_paused(yeti);
        break;

    case GAME_MODE_CREDITS    :
        screen_credits(yeti);
        break;

    case GAME_MODE_OPTIONS    :
        screen_view_options(yeti);
        break;

    case GAME_MODE_SKILL      :
        screen_skill(yeti);
        break;

    case GAME_MODE_EPISODE    :
        screen_episode(yeti);
        break;

    case GAME_MODE_LOADGAME   :
        screen_load_game(yeti);
        break;

    case GAME_MODE_QUITGAME   :
        screen_quit_game(yeti);
        break;

    case GAME_MODE_ENDLEVEL   :
        screen_end_level(yeti);
        break;
    }

    //image_draw_at(yeti->viewport.back->pixels[0], spr_test, 0, 0, 0);
}

void game_tick(yeti_t *yeti)
{
    switch (game_mode(&yeti->game)) {
    case GAME_MODE_PUBLISHER: {
        yeti->game.tick += 1;

        if (KEYPRESS_A(yeti)) {
            game_goto(&yeti->game, GAME_MODE_YETI3D);
        }

        break;
    }

    case GAME_MODE_YETI3D: {
        yeti->game.tick += 2;

        if (KEYPRESS_A(yeti)) {
            game_goto(&yeti->game, GAME_MODE_TITLE);
        }

        break;
    }

    case GAME_MODE_TITLE: {
        yeti->game.tick += 2;

        if (KEYPRESS_A(yeti)) {
            game_goto(&yeti->game, GAME_MODE_MENU);
        }

        break;
    }

    case GAME_MODE_MENU: {
        game_cycle(&yeti->game, 2);

        if (KEYPRESS_A(yeti)) {
            switch (yeti->game.menu.main) {
            case 0:
                game_goto(&yeti->game, GAME_MODE_EPISODE);
                break;

            case 1:
                game_goto(&yeti->game, GAME_MODE_OPTIONS);
                break;

            case 2:
                game_goto(&yeti->game, GAME_MODE_LOADGAME);
                break;

            case 3:
                game_goto(&yeti->game, GAME_MODE_CREDITS);
                break;
            }
        }

        game_menu(yeti, &yeti->game.menu.main);

        if (KEYPRESS_B(yeti)) {
            game_goto(&yeti->game, GAME_MODE_TITLE);
        }

        break;
    }

    case GAME_MODE_EPISODE: {
        game_cycle(&yeti->game, 2);

        if (KEYPRESS_A(yeti)) {
            game_goto(&yeti->game, GAME_MODE_SKILL);
        }

        if (KEYPRESS_B(yeti)) {
            game_goto(&yeti->game, GAME_MODE_MENU);
        }

        game_menu(yeti, &yeti->game.menu.episode);
        break;
    }

    case GAME_MODE_SKILL: {
        game_cycle(&yeti->game, 2);

        if (KEYPRESS_A(yeti)) {
            game_goto(&yeti->game, GAME_MODE_START_LEVEL);
        }

        if (KEYPRESS_B(yeti)) {
            game_goto(&yeti->game, GAME_MODE_EPISODE);
        }

        game_menu(yeti, &yeti->game.menu.skill);
        break;
    }

    case GAME_MODE_PLAY: {
        yeti_tick(yeti);

        if (KEYPRESS_SELECT(yeti)) {
            game_goto(&yeti->game, GAME_MODE_PAUSED);
        }

        break;
    }

    case GAME_MODE_START_LEVEL: {
        game_load_map(yeti, maps[yeti->game.episode]);
        game_goto(&yeti->game, GAME_MODE_PLAY);
        break;
    }

    case GAME_MODE_PAUSED: {
        if (KEYPRESS_SELECT(yeti)) {
            game_goto(&yeti->game, GAME_MODE_PLAY);
        }

        if (KEYPRESS_A(yeti)) {
            switch (yeti->game.menu.paused) {
            case 0:
                game_goto(&yeti->game, GAME_MODE_PLAY);
                break;

            case 1:
                game_goto(&yeti->game, GAME_MODE_OPTIONS);
                break;

            case 2:
                game_goto(&yeti->game, GAME_MODE_LOADGAME);
                break;

            case 3:
                game_goto(&yeti->game, GAME_MODE_QUITGAME);
                break;
            }
        }

        if (KEYPRESS_B(yeti)) {
            game_goback(&yeti->game);
        }

        game_menu(yeti, &yeti->game.menu.paused);
        break;
    }

    case GAME_MODE_CREDITS: {
        game_cycle(&yeti->game, 2);

        if (KEYPRESS_A(yeti)) {
            game_goback(&yeti->game);
        }

        if (KEYPRESS_B(yeti)) {
            game_goback(&yeti->game);
        }

        break;
    }

    case GAME_MODE_OPTIONS: {
        game_cycle(&yeti->game, 2);

        if (KEYPRESS_A(yeti)) {
            game_goback(&yeti->game);
        }

        if (KEYPRESS_B(yeti)) {
            game_goback(&yeti->game);
        }

        game_menu(yeti, &yeti->game.menu.option);
        break;
    }

    case GAME_MODE_LOADGAME: {
        game_cycle(&yeti->game, 2);

        if (KEYPRESS_A(yeti)) {
            game_goback(&yeti->game);
        }

        if (KEYPRESS_B(yeti)) {
            game_goback(&yeti->game);
        }

        break;
    }

    case GAME_MODE_QUITGAME: {
        game_cycle(&yeti->game, 2);

        if (KEYPRESS_A(yeti)) {
            game_goback(&yeti->game);
        }

        if (KEYPRESS_B(yeti)) {
            game_goback(&yeti->game);
        }

        break;
    }

    case GAME_MODE_ENDLEVEL: {
        pixel_buffer_clear(yeti->viewport.back);
        yeti_tick(yeti);

        if (KEYPRESS_A(yeti)) {
            yeti->game.episode++;

            if (yeti->game.episode > 11) {
                yeti->game.episode = 0;
            }

            game_goto(&yeti->game, GAME_MODE_START_LEVEL);
        }

        break;
    }
    }

    keyboard_tick(&yeti->keyboard);
}

/*
** Name: game_loop
** Desc: The generic game loop function. Only used by platforms that can run
**       the engine at full speed.
*/
void game_loop(yeti_t *yeti)
{
    game_tick(yeti);
    game_draw(yeti);
}


