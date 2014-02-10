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

#ifndef __ENTITIES_H__
#define __ENTITIES_H__

/******************************************************************************/

#include "../y3d/y3d_yeti.h"

/******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/

extern void boost_init(yeti_t *yeti, entity_t *e);
extern void quad_init(yeti_t *yeti, entity_t *e);
extern void teleporter_init(yeti_t *yeti, entity_t *e);

extern void sheba_init(yeti_t *yeti, entity_t *e);
extern void torch0_init(yeti_t *yeti, entity_t *e);
extern void torch1_init(yeti_t *yeti, entity_t *e);

extern void barrontree_init(yeti_t *yeti, entity_t *e);
extern void leafyplant_init(yeti_t *yeti, entity_t *e);
extern void cactus_init(yeti_t *yeti, entity_t *e);
extern void potplant_init(yeti_t *yeti, entity_t *e);

extern void overpass_init(yeti_t *yeti, entity_t *e);
extern void bridge_init(yeti_t *yeti, entity_t *e);
extern void pillar_init(yeti_t *yeti, entity_t *e);
extern void emptypot_init(yeti_t *yeti, entity_t *e);
extern void pinechair_init(yeti_t *yeti, entity_t *e);
extern void crate_init(yeti_t *yeti, entity_t *e);

extern void bullet_shoot(yeti_t *yeti, entity_t *e);
extern void bullet_tick(yeti_t *yeti, entity_t *e);
extern void explode_init(yeti_t *yeti, entity_t *e);
extern void splat_init(yeti_t *yeti, entity_t *e);
extern void death_init(yeti_t *yeti, entity_t *e);

/******************************************************************************/

#ifdef __cplusplus
};
#endif

/******************************************************************************/

#endif
