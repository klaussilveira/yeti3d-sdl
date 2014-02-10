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

#ifndef __QUAKE_H__
#define __QUAKE_H__

/******************************************************************************/

#define MD2_ANIM_STAND          0
#define MD2_ANIM_RUN            1
#define MD2_ANIM_ATTACK         2
#define MD2_ANIM_PAIN1          3
#define MD2_ANIM_PAIN2          4
#define MD2_ANIM_PAIN3          5
#define MD2_ANIM_JUMP           6
#define MD2_ANIM_FLIPOFF        7
#define MD2_ANIM_SALUTE         8
#define MD2_ANIM_TAUNT          9
#define MD2_ANIM_WAVE           10
#define MD2_ANIM_POINT          11
#define MD2_ANIM_CROUCH_STAND   12
#define MD2_ANIM_CROUCH_WALK    13
#define MD2_ANIM_CROUCH_ATTACK  14
#define MD2_ANIM_CROUCH_PAIN    15
#define MD2_ANIM_CROUCH_DEATH   16
#define MD2_ANIM_DEATH1         17
#define MD2_ANIM_DEATH2         18
#define MD2_ANIM_DEATH3         19

typedef struct {
    float u, v;
    int vindex;
} md2command_t;

typedef struct {
    int magic;
    int version;
    int skinWidth;
    int skinHeight;
    int frameSize;
    int numSkins;
    int numVertices;
    int numTexCoords;
    int numTriangles;
    int numGlCommands;
    int numFrames;
    int offsetSkins;
    int offsetTexCoords;
    int offsetTriangles;
    int offsetFrames;
    int offsetGlCommands;
    int offsetEnd;
} model_t;

typedef struct {
    short vertexIndices[3];
    short textureIndices[3];
} triangle_t;

// NOTE: These must be unsigned to support 0..255 fixpt UV's

typedef struct {
    unsigned short s, t;
} textureCoordinate_t;

typedef struct {
    u8 vertex[3];
    u8 lightNormalIndex;
} triangleVertex_t;

typedef struct {
    float scale[3];
    float translate[3];
    char name[16];
    triangleVertex_t vertices[1];
} frame_t;

/******************************************************************************/

#endif
