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

#include "y3d_animation.h"
#include "y3d_fixed.h"

/******************************************************************************/

/*
** Name: animation_init
** Desc:
*/
void animation_init(animation_t *a, int mode, int start, int length, int speed, int loops)
{
    a->mode   = mode;
    a->start  = i2f(start);
    a->length = i2f(length);
    a->speed  = speed;
    a->end    = a->start + a->length;
    a->pos    = a->start;
    a->nloops = loops;
}

/*
** Name: animation_tick
** Desc:
*/
void animation_tick(animation_t *a)
{
    if (a->length && a->nloops) {
        a->pos += a->speed;

        if (a->pos < a->start) {
            if (--a->nloops) {
                a->pos += a->length;
            } else {
                a->pos = a->start;
            }
        }

        if (a->pos >= a->end) {
            if (--a->nloops) {
                a->pos -= a->length;
            } else {
                a->pos = a->end - 1;
            }
        }

        if (a->nloops > (LOOP_FOREVER - 4)) {
            a->nloops = LOOP_FOREVER;
        }
    }
}

/*
** Name: animation_mode
** Desc:
*/
void animation_mode(animation_t *a, int mode, int speed, int loops)
{
    const u8 frames[][2] = {
        {  0, 40},    //MD2_ANIM_STAND
        { 40,  6},    //MD2_ANIM_RUN
        { 46,  8},    //MD2_ANIM_ATTACK
        { 54,  4},    //MD2_ANIM_PAIN1
        { 58,  4},    //MD2_ANIM_PAIN2
        { 62,  4},    //MD2_ANIM_PAIN3
        { 66,  6},    //MD2_ANIM_JUMP
        { 72, 12},    //MD2_ANIM_FLIPOFF
        { 84, 11},    //MD2_ANIM_SALUTE
        { 95, 17},    //MD2_ANIM_TAUNT
        {112, 11},    //MD2_ANIM_WAVE
        {123, 12},    //MD2_ANIM_POINT
        {135, 19},    //MD2_ANIM_CROUCH_STAND
        {154,  6},    //MD2_ANIM_CROUCH_WALK
        {160,  9},    //MD2_ANIM_CROUCH_ATTACK
        {169,  4},    //MD2_ANIM_CROUCH_PAIN
        {173,  5},    //MD2_ANIM_CROUCH_DEATH
        {178,  6},    //MD2_ANIM_DEATH1
        {184,  6},    //MD2_ANIM_DEATH2
        {190,  8}     //MD2_ANIM_DEATH3
    };
    animation_init(a, mode, frames[mode][0], frames[mode][1], speed, loops);
}

/*
** Name: animation_get_frames
** Desc:
*/
void animation_get_frames(animation_t *animation, int *f1, int *f2)
{
    *f1 = f2i(animation->pos);
    *f2 = f2i(animation->pos + FIXCEIL);

    if (*f2 >= f2i(animation->end)) {
        *f2 = animation->nloops > 1 ? f2i(animation->start) : *f1;
    }
}
