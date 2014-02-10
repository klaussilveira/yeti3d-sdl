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

#include "y3d_engine.h"
#include "y3d_quake.h"
#include "y3d_draw.h"
#include "y3d_image.h"

/******************************************************************************/

/*
** Name: yeti_sprite_draw
** Desc:
*/
void yeti_sprite_draw(yeti_t *yeti, entity_t *entity)
{
    u16 *image = entity->visual.data;
    int w = f2i(IMAGE_WIDTH(image) * entity->visual.width);
    int h = f2i(IMAGE_HEIGHT(image) * entity->visual.height);
    image_draw_di(
        yeti->viewport.back->pixels[0],
        entity->rx - w, entity->ry + h, entity->rx + w, entity->ry - h, entity->rz,
        IMAGE(image), entity->visual.mode);
}

/******************************************************************************/

/*
** Name: yeti_model_draw
** Desc: Draws a model given an entity. The data is taken from e->visual.data.
**       The model is rotated and positioned at the entity pos.
*/
void yeti_model_draw(yeti_t *yeti, entity_t *entity)
{
    cell_t *cell = ENTITY_CELL(yeti, entity);
    s16 *model = (s16 *) entity->visual.data;
    matrix_t om, wm;
    triangle_t *triangle;
    bucket_node_t nodes[YETI_MODEL_TRANGLE_MAX];
    short buckets[YETI_MODEL_BUCKET_MAX];
    int i, j, k, n, y;
    int offsetTexCoords = model[0] + model[1] * model[5];
    int offsetTriangles = offsetTexCoords + (model[3] << 1);
    triangle_t *triangles = (triangle_t *) &model[offsetTriangles];
    // Calculate the object matrix for the entity.
    matrix_rotate_object(om,
                         entity->visual.altpitch ? 0 : entity->p,
                         entity->visual.altroll  ? 0 : entity->r,
                         entity->visual.altturn  ? 0 : entity->t,
                         entity->x - yeti->camera->x,
                         entity->z - yeti->camera->z,
                         entity->y - yeti->camera->y);
    // Multiple the object matrix by the camera matrix to get the world
    // matrix.
    matrix_multiply(wm, om, yeti->m);

    if (entity->rz < yeti->visual.threshold_perspective && entity->animation.speed) {
        animation_get_frames(&entity->animation, &i, &j);
        {
            s16 *v1 = &model[model[0] + model[5] * i];
            s16 *v2 = &model[model[0] + model[5] * j];

            for (i = 0; i < model[2]; i++, v1 += 3, v2 += 3) {
                int v[3];
                v[0] = FIXLERP(v1[0], v2[0], fixfrac(entity->animation.pos));
                v[1] = FIXLERP(v1[1], v2[1], fixfrac(entity->animation.pos));
                v[2] = FIXLERP(v1[2], v2[2], fixfrac(entity->animation.pos));
                yeti->verts[i].x = FIXDOT(wm[0], v) + wm[0][3];
                yeti->verts[i].y = FIXDOT(wm[1], v) + wm[1][3];
                yeti->verts[i].z = FIXDOT(wm[2], v) + wm[2][3];
                VERTEX_PROJECT_SLOW(&yeti->verts[i]);
            }
        }
    } else {
        s16 *v1 = &model[model[0] + model[5] * f2i(entity->animation.pos)];

        for (i = 0; i < model[2]; i++, v1 += 3) {
            yeti->verts[i].x = FIXDOT(wm[0], v1) + wm[0][3];
            yeti->verts[i].y = FIXDOT(wm[1], v1) + wm[1][3];
            yeti->verts[i].z = FIXDOT(wm[2], v1) + wm[2][3];
            VERTEX_PROJECT_SLOW(&yeti->verts[i]);
        }
    }

    memset(buckets, 0, sizeof(buckets));
    y = entity->rz * 3;

    for (i = 0, n = 1; i < model[4]; i++) {
        triangle = &triangles[i];
        k =
            yeti->verts[triangle->vertexIndices[0]].z +
            yeti->verts[triangle->vertexIndices[1]].z +
            yeti->verts[triangle->vertexIndices[2]].z ;
        j = ((y - k) >> 6) + (YETI_MODEL_BUCKET_MAX >> 1);
        j = CLAMP(j, 0, YETI_MODEL_BUCKET_MAX - 1);
        nodes[n].next = buckets[j];
        nodes[n].index = i;
        buckets[j] = n++;
    }

    for (j = 0; j < YETI_MODEL_BUCKET_MAX; j++) {
        for (i = buckets[j]; i; i = nodes[i].next) {
            polyclip_t p, q;
            triangle = &triangles[nodes[i].index];
            p[0] = &yeti->verts[triangle->vertexIndices[0]];
            p[1] = &yeti->verts[triangle->vertexIndices[1]];
            p[2] = &yeti->verts[triangle->vertexIndices[2]];

            if (POLYCLIP_IS_VISIBLE(p)) {
                textureCoordinate_t *texcoords = (textureCoordinate_t *) &model[offsetTexCoords];
                p[0]->u = texcoords[triangle->textureIndices[0]].s;
                p[0]->v = texcoords[triangle->textureIndices[0]].t;
                p[1]->u = texcoords[triangle->textureIndices[1]].s;
                p[1]->v = texcoords[triangle->textureIndices[1]].t;
                p[2]->u = texcoords[triangle->textureIndices[2]].s;
                p[2]->v = texcoords[triangle->textureIndices[2]].t;
#ifdef __PATCH_MODEL_POLY__
                model_poly(yeti, p, (u16 *) &model[offsetTriangles + model[4] * 6]);
#else

                for (k = n = 3; k--; p[k]->d = p[k]->z - 32);

                if ((n = polycliper(q, p, n, &yeti->vpool)) > 2) {
                    polygon_t polygon;

                    for (k = n; k--;) {
                        polygon[k].x = q[k]->sx;
                        polygon[k].y = q[k]->sy;
                        polygon[k].u = q[k]->u;
                        polygon[k].v = q[k]->v;
                    }

                    if (entity->visual.mode == DRAW_MODE_COPY) {
                        if (cell->lit < i2f(8)) {
                            tmap_16_cs(
                                polygon, n, yeti->viewport.back->pixels[0],
                                (u16 *) &model[offsetTriangles + INTMUL6(model[4])],
                                DRAW_MODE_COPY_HALFBRIGHT);
                        } else {
                            tmap_16_cs(
                                polygon, n, yeti->viewport.back->pixels[0],
                                (u16 *) &model[offsetTriangles + INTMUL6(model[4])],
                                entity->visual.mode);
                        }
                    } else {
                        tmap_16(
                            polygon, n, yeti->viewport.back->pixels[0],
                            (u16 *) &model[offsetTriangles + INTMUL6(model[4])],
                            entity->visual.mode);
                    }
                }

#endif
            }
        }
    }
}

/******************************************************************************/


/*
** Name: yeti_quad_draw
** Desc: Renders a single quad/rectangle of a cell using the given texture
**       mapping mode and texture id. The mapping mode changes depending
**       of if this is a north-south wall, east-west wall or ceiling/floor.
** Note: This is one very long function. Its the result of ~3 functions
**       merged into one. Since the variable usage of each was similar, I
**       achieved an overall speed boost.
*/
void yeti_quad_draw(yeti_t *yeti, cell_t *basecell, int tid, quadrant_t quad, int texgen)
{
    int i, n, v[3];
    polyclip_t p, a;
    entity_t *entity;
    vertex_t *vertex;

    for (n = 4; n--;) {
        vec3_t *vec3 = &quad[n];
        cell_t *cell = &yeti->map.item[f2i(vec3->z)][f2i(vec3->x)];
        v[0] = vec3->x - yeti->camera->x;
        v[1] = vec3->y - yeti->camera->z;
        v[2] = vec3->z - yeti->camera->y;

        switch (cell->wwi) {
        case 1: {
            v[0] += FIXHALF;
            v[2] += FIXHALF;
            break;
        }

        case 2: {
            v[0] -= FIXHALF;
            v[2] += FIXHALF;
            break;
        }

        case 3: {
            v[0] -= FIXHALF;
            v[2] -= FIXHALF;
            break;
        }

        case 4: {
            v[0] += FIXHALF;
            v[2] -= FIXHALF;
            break;
        }

        case 5: {
            v[0] += FIXHALF;
            break;
        }

        case 6: {
            v[0] -= FIXHALF;
            break;
        }

        case 7: {
            v[2] += FIXHALF;
            break;
        }

        case 8: {
            v[2] -= FIXHALF;
            break;
        }
        }

        if (CELL_IS_MARKED(cell)) {
            for (i = cell->vcache_size; i--;) {
                if (cell->vcache[i]->i == vec3->y) {
                    vertex = cell->vcache[i];
                    goto skip_transformation;
                }
            }
        } else {
            CELL_MARK(cell);
            cell->vcache_size = 0;
        }

        vertex    = VERTEX_POOL_GET(&yeti->vpool);
        vertex->i = vec3->y;
        vertex->x = FIXDOT(yeti->m[0], v);
        vertex->y = yeti->is2d ? v[1] : FIXDOT(yeti->m[1], v);
        vertex->z = FIXDOT(yeti->m[2], v);
        i = cell->lit;// - ABS(vertex->y);

        for (entity = cell->entities; entity; entity = entity->next) {
            if (entity->emitlight) {
                i = CELL_LIGHT_MAX;
                break;
            }

            if (entity->castshadow) {
                i -= i2f(16);
                break;
            }
        }

        if (cell->sw2 & CELL_SW2_HALFBRIGHT) {
            i >>= 1;
        }

        switch (cell->sw2 & CELL_SW2_LIGHT_MASK) {
        case CELL_SW2_LIGHT1:
            i += fixsin(yeti->keyboard.tick << 11) << 2;
            break;

        case CELL_SW2_LIGHT2:
            i += (yeti->keyboard.tick & 20) << 8;
            break;

        case CELL_SW2_LIGHT3:
            i += fixsin(((vec3->x + vec3->z) << 4) + (yeti->keyboard.tick << 11)) << 4;
            break;
        }

        i *= yeti->visual.brightness;

        if (i < CELL_LIGHT_MIN) {
            i = CELL_LIGHT_MIN;
        }

        if (i > CELL_LIGHT_MAX) {
            i = CELL_LIGHT_MAX;
        }

        vertex->l = i;
        VERTEX_PROJECT_SLOW(vertex);

        if (cell->vcache_size < CELL_VCACHE_MAX) {
            cell->vcache[cell->vcache_size++] = vertex;
        }

skip_transformation:
        p[n] = vertex;
        tid = CLAMP(tid, 0, TEXTURE_MAX - 1);
        v[0] += yeti->camera->x;
        v[1] += yeti->camera->z;
        v[2] += yeti->camera->y;
#define U   (yeti->surfaces.item[tid].xsize)
#define V   (yeti->surfaces.item[tid].ysize)
#define UU  (yeti->surfaces.item[tid].xpan)
#define VV  (yeti->surfaces.item[tid].ypan)

        switch (texgen) {
        case YETI_TEXGEN_LEFT:
            vertex->u = (v[2] + UU) << U;
            vertex->v = (v[1] + VV) << V;
            break;

        case YETI_TEXGEN_RIGHT:
            vertex->u = (-v[2] + UU) << U;
            vertex->v = (v[1] + VV) << V;
            break;

        case YETI_TEXGEN_FRONT:
            vertex->u = (v[0] + UU) << U;
            vertex->v = (v[1] + VV) << V;
            break;

        case YETI_TEXGEN_BACK:
            vertex->u = (-v[0] + UU) << U;
            vertex->v = (v[1] + VV) << V;
            break;

        case YETI_TEXGEN_TOP:
        case YETI_TEXGEN_BOT:
            vertex->u = (v[0] + UU) << U;
            vertex->v = (v[2] + VV) << V;
            break;
        }
    }

#undef U
#undef V
#undef UU
#undef VV
#ifdef __PATCH_DRAW_TEXTURE__
    draw_texture(yeti, p, tid, yeti->lighting[CELL_LUA(basecell)], yeti->surfaces.item[tid].mode);
#else

    for (i = n = 4; i--;) {
        vertex = p[i];

        if (VERTEX_IS_OUTSIDE(vertex)) {
            for (i = n; i--; p[i]->d = p[i]->x + p[i]->z);

            n = polycliper(a, p, n, &yeti->vpool); // Left

            for (i = n; i--; a[i]->d = a[i]->z - a[i]->x);

            n = polycliper(p, a, n, &yeti->vpool); // Right

            for (i = n; i--; p[i]->d = p[i]->y + p[i]->z);

            n = polycliper(a, p, n, &yeti->vpool); // Top

            for (i = n; i--; a[i]->d = a[i]->z - a[i]->y);

            n = polycliper(p, a, n, &yeti->vpool); // Bottom
            break;
        }
    }

    if (n > 2) {
        polygon_t polygon;

        for (i = n; i--;) {
            polygon[i].x = p[i]->sx;
            polygon[i].y = p[i]->sy;
            polygon[i].z = p[i]->z;
            polygon[i].u = p[i]->u;
            polygon[i].v = p[i]->v;
            polygon[i].l = p[i]->l;
        }

        if (TEXTURE_IS_BRIGHT(tid)) {
            for (i = n; i--;) {
                polygon[i].l += i2f(16);// CELL_LIGHT_SKY;

                if (polygon[i].l > CELL_LIGHT_MAX) {
                    polygon[i].l = CELL_LIGHT_MAX;
                }
            }
        }

        if (yeti->skyimage && tid == TEXTURE_SKY) {
            tmap_16_sky(
                polygon, n,
                yeti->viewport.back->pixels[0],
                yeti->skyimage, ((yeti->camera->t >> 7) /* + (yeti->keyboard.tick << 0)*/) & 511,
                yeti->skyimage, ((yeti->camera->t >> 7) /* + (yeti->keyboard.tick << 0)*/) & 511);
        } else {
            int zmin = p[0]->z;

            if (p[1]->z < zmin) {
                zmin = p[1]->z;
            }

            if (p[2]->z < zmin) {
                zmin = p[2]->z;
            }

            if (p[3]->z < zmin) {
                zmin = p[3]->z;
            }

            if (zmin < yeti->visual.threshold_perspective) {
                if (
                    zmin < yeti->visual.threshold_dithering &&
                    (
                        yeti->surfaces.item[tid].mode == DRAW_MODE_COPY ||
                        yeti->surfaces.item[tid].mode == DRAW_MODE_BLEND
                    )) {
                    tmap_8_horz_pc(
                        polygon, n,
                        yeti->viewport.back->pixels[0],
                        yeti->textures[tid][0],
                        yeti->luas[CELL_LUA(basecell)],
                        yeti->surfaces.item[tid].mode + 1);
                } else {
                    tmap_8_horz_pc(
                        polygon, n,
                        yeti->viewport.back->pixels[0],
                        yeti->textures[tid][0],
                        yeti->luas[CELL_LUA(basecell)],
                        yeti->surfaces.item[tid].mode);
                }
            } else {
                tmap_8_horz(
                    polygon, n,
                    yeti->viewport.back->pixels[0],
                    yeti->textures[tid][0],
                    yeti->luas[CELL_LUA(basecell)],
                    yeti->surfaces.item[tid].mode);
            }
        }

        if (CELL_IS_SELECTED(basecell)) {
            draw_poly_line_nc(yeti->viewport.back, p, n,
                              CELL_IS_TERRAIN(basecell) || CELL_IS_CAVE(basecell) ?
                              COLOR_LIME : COLOR_RED);
        }
    }

#endif
}

/******************************************************************************/

/*
** Name: yeti_quad
** Desc: This is a intermediate step to the quad processor stage. The idea is
**       to allow pre-quad effects like liquid and multi-texturing. Slower
**       processors can bypass the high-end features.
*/
void yeti_quad(yeti_t *yeti, cell_t *basecell, int tid, quadrant_t quad, int texgen)
{
    int i, mode;

    if (texgen == YETI_TEXGEN_BOT) {
        if (TEXTURE_IS_LIQUID(tid)) {
            i = yeti->keyboard.tick << 5;

            if (yeti->surfaces.item[tid].mode == DRAW_MODE_BLEND) {
                yeti_quad_draw(yeti, basecell, 5, quad, texgen);
            }

            quad[0].y += (fixsin((quad[0].x + quad[0].z + i) << 5) >> 3) + 32;
            quad[1].y += (fixsin((quad[1].x + quad[1].z + i) << 5) >> 3) + 32;
            quad[2].y += (fixsin((quad[2].x + quad[2].z + i) << 5) >> 3) + 32;
            quad[3].y += (fixsin((quad[3].x + quad[3].z + i) << 5) >> 3) + 32;
        }
    }

    yeti_quad_draw(yeti, basecell, tid, quad, texgen);

    if (texgen == YETI_TEXGEN_BOT) {
        if (CELL_IS_OVERPASS(basecell)) {
            quad[0].y = quad[1].y = quad[2].y = quad[3].y = basecell->bot + basecell->eos;
            mode = yeti->surfaces.item[40].mode;
            yeti->surfaces.item[40].mode = DRAW_MODE_BLEND;

            if (yeti->camera->z > quad[0].y) {
                yeti_quad_draw(yeti, basecell, 40, quad, texgen);
            } else {
                i = quad[0].x;
                quad[0].x = quad[1].x;
                quad[1].x = i;
                i = quad[2].x;
                quad[2].x = quad[3].x;
                quad[3].x = i;
                yeti_quad_draw(yeti, basecell, 122, quad, texgen);
                i = quad[0].x;
                quad[0].x = quad[1].x;
                quad[1].x = i;
                i = quad[2].x;
                quad[2].x = quad[3].x;
                quad[3].x = i;
            }

            yeti->surfaces.item[40].mode = mode;
        }
    }

    /*
      if (
        !TEXTURE_IS_BRIGHT(tid) &&
        !TEXTURE_IS_LIQUID(tid) &&
        yeti->surfaces.item[tid].mode != DRAW_MODE_COLORKEY)
      {
        int ttt = 98;
        surface_t* surface = &yeti->surfaces.item[ttt];
        mode = surface->mode;
        surface->mode = DRAW_MODE_BLEND;
        surface->xsize -= 0;
        surface->ysize -= 0;
        yeti_quad_draw(yeti, basecell, ttt, quad, texgen);
        surface->xsize += 0;
        surface->ysize += 0;
        surface->mode = mode;
      }
    */
}

/******************************************************************************/

/*
** Name: yeti_build_vis
** Desc: Builds a VIS given the current camera location. Returns the list of
**       visible cells and the total number of cells visible.
** Note: This code was placed into a seperate function to prevent stack
**       blowouts. The array of rays is only used once.
*/
void yeti_build_vis(yeti_t *yeti, cell_vis_t *vis)
{
    ray_t rays[YETI_RAY_MAX], *ray;
    cell_t *cell;
    int x = yeti->camera->x << 2;
    int y = yeti->camera->y << 2;
    int i = YETI_RAY_MAX;
    int j = (YETI_RAY_MAX >> 1) * YETI_RAY_WIDTH + yeti->camera->t;

    for (; i--; j -= YETI_RAY_WIDTH) {
        ray    = &rays[i];
        ray->x = x;
        ray->xx = fixsin(j);
        ray->y = y;
        ray->yy = fixcos(j);
    }

    CELL_UNMARK_ALL();
    vis->length = 0;

    for (j = YETI_RAY_MAX; j && vis->length < YETI_CELL_MAX;) {
        for (ray = rays, i = YETI_RAY_MAX; i--; ray++) {
            if (ray->x) {
                cell = &yeti->map.item[y = f2i(ray->y >> 2)][x = f2i(ray->x >> 2)];

                if (cell->xlod < 0 || cell->ylod < 0) {
                    cell = &yeti->map.item[y += cell->ylod][x += cell->xlod];
                }

                if (CELL_IS_OPAQUE(cell)) {
                    j--;
                    ray->x = 0;
                } else {
                    if (!CELL_IS_MARKED(cell)) {
                        CELL_MARK(cell);
                        vis->item[vis->length].x = x;
                        vis->item[vis->length].y = y;

                        if (++vis->length == YETI_CELL_MAX) {
                            pixel_buffer_clear(yeti->viewport.back);
                            break;
                        }
                    }

                    ray->x += ray->xx;
                    ray->y += ray->yy;
                }
            }
        }
    }
}

/******************************************************************************/

/*
** Name: yeti_draw
** Desc: Renders the Yeti3D world. This one function handles all the visiblity
**       and renderings. Its the main control center for the entire engine.
*/
void yeti_draw(yeti_t *yeti)
{
    entity_t *camera = yeti->camera;
    int i, x1, y1, x2, y2, v[3];
    cell_vis_t vis;
    entity_t *e1;
    entity_t *e2;
    quadrant_t q;
    cell_t *camcell;
    cell_t *c;
    cell_t *d;
    yeti->is2d = !camera->p && !camera->r;
    matrix_rotate_world(yeti->m, -camera->r, -camera->p, -camera->t);
    i = reciprocal[YETI_VIEWPORT_WIDTH] * YETI_VIEWPORT_HEIGHT;
    yeti->m[0][0] = r2i(yeti->m[0][0] * i);
    yeti->m[0][1] = r2i(yeti->m[0][1] * i);
    yeti->m[0][2] = r2i(yeti->m[0][2] * i);
    yeti_build_vis(yeti, &vis);
    camcell = &yeti->map.item[y1 = f2i(camera->y)][x1 = f2i(camera->x)];

    if (camcell->xlod < 0 || camcell->ylod < 0) {
        camcell = &yeti->map.item[y1 + camcell->ylod][x1 + camcell->xlod];
    }

    // Merge the entities into the world map.

    for (i = yeti->entities.length; i--;) {
        e1 = &yeti->entities.item[i];

        if (e1->visual.data) {
            if (e1->alwaysvisible) {
                c = camcell;
            } else {
                c = &yeti->map.item[y1 = f2i(e1->y)][x1 = f2i(e1->x)];

                if (c->xlod < 0 || c->ylod < 0) {
                    c = &yeti->map.item[y1 + c->ylod][x1 + c->xlod];
                }
            }

            if (CELL_IS_MARKED(c)) {
                if (e1->alwaysontop) {
                    c = camcell;
                }

                v[0] = e1->x - camera->x;
                v[1] = e1->z - camera->z;
                v[2] = e1->y - camera->y;
                e1->rx = FIXDOT(yeti->m[0], v);
                e1->ry = yeti->is2d ? v[1] : FIXDOT(yeti->m[1], v);
                e1->rz = FIXDOT(yeti->m[2], v);

                if (!c->entities || e1->z < c->entities->rz) {
                    e1->next = c->entities;
                    c->entities = e1;
                } else {
                    for (e2 = c->entities; e2->next && e1->rz < e2->next->rz; e2 = e2->next);

                    e1->next = e2->next;
                    e2->next = e1;
                }
            }
        }
    }

    CELL_UNMARK_ALL();

    for (i = vis.length; i--;) {
#define DRAW_WALL(TEXGEN,CELL,TID,TOP1,TOP2,BOT1,BOT2)\
q[0].y=TOP1;q[1].y=BOT1;q[2].y=BOT2;q[3].y=TOP2;\
if (q[1].y<q[0].y||q[2].y<q[3].y) yeti_quad(yeti, CELL, TID, q, TEXGEN);
        c  = &yeti->map.item[y1 = vis.item[i].y][x1 = vis.item[i].x];
        x2 = (x1 = i2f(x1)) + i2f(c->xlod);
        y2 = (y1 = i2f(y1)) + i2f(c->ylod);

        // Render Right Wall

        if (camera->x < x2 || CELL_IS_WARPED(c)) {
            d = &c[c->xlod];
            q[0].x = q[1].x = q[2].x = q[3].x = x2;
            q[0].z = q[1].z = y1;
            q[2].z = q[3].z = y2;
            DRAW_WALL(YETI_TEXGEN_RIGHT, d, d->wtb,
                      d->bots[3], d->bots[0], c->bots[2], c->bots[1]);
            DRAW_WALL(YETI_TEXGEN_RIGHT, d, d->wtt, c->tops[2], c->tops[1],
                      MAX(d->tops[3], c->bots[2]), MAX(d->tops[0], c->bots[1]));
        }

        // Render Left Wall

        if (camera->x > x1 || CELL_IS_WARPED(c)) {
            d = &c[-1];
            q[0].x = q[1].x = q[2].x = q[3].x = x1;
            q[0].z = q[1].z = y2;
            q[2].z = q[3].z = y1;
            DRAW_WALL(YETI_TEXGEN_LEFT, d, d->wtb,
                      d->bots[1], d->bots[2], c->bots[0], c->bots[3]);
            DRAW_WALL(YETI_TEXGEN_LEFT, d, d->wtt, c->tops[0], c->tops[3],
                      MAX(d->tops[1], c->bots[0]), MAX(d->tops[2], c->bots[3]));
        }

        // Render Front Wall

        if (camera->y < y2 || CELL_IS_WARPED(c)) {
            d = &c[c->ylod * MAP_WIDTH];
            q[0].z = q[1].z = q[2].z = q[3].z = y2;
            q[0].x = q[1].x = x2;
            q[2].x = q[3].x = x1;
            DRAW_WALL(YETI_TEXGEN_FRONT, d, d->wtb,
                      d->bots[2], d->bots[3], c->bots[1], c->bots[0])
            DRAW_WALL(YETI_TEXGEN_FRONT, d, d->wtt, c->tops[1], c->tops[0],
                      MAX(d->tops[2], c->bots[1]), MAX(d->tops[3], c->bots[0]));
        }

        // Render Back Wall

        if (camera->y > y1 || CELL_IS_WARPED(c)) {
            d = &c[-MAP_WIDTH];
            q[0].z = q[1].z = q[2].z = q[3].z = y1;
            q[0].x = q[1].x = x1;
            q[2].x = q[3].x = x2;
            DRAW_WALL(YETI_TEXGEN_BACK, d, d->wtb,
                      d->bots[0], d->bots[1], c->bots[3], c->bots[2]);
            DRAW_WALL(YETI_TEXGEN_BACK, d, d->wtt, c->tops[3], c->tops[2],
                      MAX(d->tops[0], c->bots[3]), MAX(d->tops[1], c->bots[2]));
        }

#undef DRAW_WALL

        // Render cell ceiling and floor

        if (!CELL_IS_SOLID(c)) {
            q[0].x = q[3].x = x1;
            q[1].x = q[2].x = x2;

            // Cell Bottom (Floor)

            if (camera->z > c->bot || CELL_IS_TERRAIN(c)) {
                q[0].y = c->bots[0];
                q[1].y = c->bots[1];
                q[2].y = c->bots[2];
                q[3].y = c->bots[3];
                q[0].z = q[1].z = y2;
                q[2].z = q[3].z = y1;
                yeti_quad(yeti, c, c->btx, q, YETI_TEXGEN_BOT);
            }

            // Cell Top (Ceiling)

            if (camera->z < c->top || CELL_IS_CAVE(c)) {
                q[0].y = c->tops[3];
                q[1].y = c->tops[2];
                q[2].y = c->tops[1];
                q[3].y = c->tops[0];
                q[0].z = q[1].z = y1;
                q[2].z = q[3].z = y2;
                yeti_quad(yeti, c, c->ttx, q, YETI_TEXGEN_TOP);
            }
        }

        for (e1 = c->entities; e1; e1 = e1->next) {
            if (e1->visual.data) {
                if (e1->visual.ondraw) {
                    e1->visual.ondraw(yeti, e1);
                } else {
                    yeti_sprite_draw(yeti, e1);
                }
            }
        }

        c->entities = 0;
    }

    if (yeti->fps) {
        char s[10];
        ansic_itos(yeti->fps, ansic_stpcpy(s, "FPS="));
        draw_string(yeti->viewport.back, 0, 0, s, 3, 3, 0, 0);
    }

    if (yeti->message.text && yeti->message.tick) {
#ifdef __GBA__
        draw_string(yeti->viewport.back,
                    (YETI_TEXTPORT_WIDTH - strlen(yeti->message.text)) >> 1,
                    YETI_TEXTPORT_HEIGHT >> 1,
                    yeti->message.text, 3, 3,
                    yeti->message.tick > 4 ? DRAW_MODE_COLORKEY : DRAW_MODE_COLORKEY_BLEND,
                    0);
#else
        draw_string(yeti->viewport.back,
                    ((YETI_TEXTPORT_WIDTH >> TEXT_SCALE) - strlen(yeti->message.text)) >> 1,
                    (YETI_TEXTPORT_HEIGHT >> TEXT_SCALE) >> 2,
                    yeti->message.text, 3, 4,
                    yeti->message.tick > 4 ? DRAW_MODE_COLORKEY : DRAW_MODE_COLORKEY_BLEND,
                    0);
#endif
    }
}

/******************************************************************************/

/*
** Name: cell_bot
** Desc: Returns the bottom position of a cell at the given entity pos.
*/
int cell_bot(cell_t *cell, entity_t *e)
{
    int bot;

    if (CELL_IS_TERRAIN(cell)) {
        int x1 = FIXLERP(cell->bots[0], cell->bots[1], fixfrac(e->x));
        int x2 = FIXLERP(cell->bots[3], cell->bots[2], fixfrac(e->x));
        bot = FIXLERP(x2, x1, fixfrac(e->y));
    } else {
        bot = cell->bot;
    }

    return e->z > bot + cell->hos ? bot + cell->eos : bot;
}

/*
** Name: cell_top
** Desc: Returns the top position of a cell at the given entity pos.
*/
int cell_top(cell_t *cell, entity_t *e)
{
    if (e->z > cell->bot && e->z < cell->bot + cell->hos) {
        return cell->bot + cell->hos;
    }

    if (CELL_IS_CAVE(cell)) {
        int x1 = FIXLERP(cell->tops[0], cell->tops[1], fixfrac(e->x));
        int x2 = FIXLERP(cell->tops[3], cell->tops[2], fixfrac(e->x));
        return FIXLERP(x2, x1, fixfrac(e->y));
    } else {
        return cell->top;
    }
}

/******************************************************************************/

/*
** Name: yeti_tick
** Desc: Stable engine tick function. Handles entity callbacks and entity to
**       world collision.
*/
void yeti_tick(yeti_t *yeti)
{
    int i, j;

    if (yeti->message.tick) {
        yeti->message.tick--;
    }

    for (i = 0; i < yeti->entities.length; i++) {
        entity_t *e1 = &yeti->entities.item[i];

        if (e1->ontick) {
            e1->ontick(yeti, e1);
        }

        e1->x += e1->xx;
        e1->y += e1->yy;
        e1->z += e1->zz;
        e1->r += e1->rr;
        e1->t += e1->tt;
        e1->p += e1->pp;

        if (e1->radius) {
            int t, b;
            cell_t *cell;
#define IS_COLLISION (e1->z<(b=cell_bot(cell,e1))||e1->z>(t=cell_top(cell,e1))||((t-b)<192))
            cell = &yeti->map.item[f2i(e1->y)][f2i(j = e1->x - e1->radius)];

            if (IS_COLLISION) {
                if (e1->onwall) {
                    e1->onwall(yeti, e1, cell);
                }

                e1->x += FIXONE - fixfrac(j);
                e1->xx = 0;
            }

            cell = &yeti->map.item[f2i(e1->y)][f2i(j = e1->x + e1->radius)];

            if (IS_COLLISION) {
                if (e1->onwall) {
                    e1->onwall(yeti, e1, cell);
                }

                e1->x -= fixfrac(j);
                e1->xx = 0;
            }

            cell = &yeti->map.item[f2i(j = e1->y - e1->radius)][f2i(e1->x)];

            if (IS_COLLISION) {
                if (e1->onwall) {
                    e1->onwall(yeti, e1, cell);
                }

                e1->y += FIXONE - fixfrac(j);
                e1->yy = 0;
            }

            cell = &yeti->map.item[f2i(j = e1->y + e1->radius)][f2i(e1->x)];

            if (IS_COLLISION) {
                if (e1->onwall) {
                    e1->onwall(yeti, e1, cell);
                }

                e1->y -= fixfrac(j);
                e1->yy = 0;
            }

#undef IS_COLLISION

            for (j = i + 1; j < yeti->entities.length; j++) {
                entity_t *e2 = &yeti->entities.item[j];

                if (e2->radius) {
                    int k = e1->radius + e2->radius;
                    int x = e1->x - e2->x;

                    if (x < k && x > -k) {
                        int y = e1->y - e2->y;

                        if (y < k && y > -k) {
                            int z = e1->z - e2->z + FIXHALF;

                            if (z < k && z > -k) {
                                if (e1->onhit) {
                                    e1->onhit(yeti, e1, e2);
                                }

                                if (e2->onhit) {
                                    e2->onhit(yeti, e2, e1);
                                }

                                if (yeti->onhit) {
                                    yeti->onhit(yeti, e1, e2);
                                }

                                if (!e1->nocollision) {
                                    if (x > 0) {
                                        e1->x += (k - x) >> 1;
                                    }

                                    if (x < 0) {
                                        e1->x += (-k - x) >> 1;
                                    }

                                    if (y > 0) {
                                        e1->y += (k - y) >> 1;
                                    }

                                    if (y < 0) {
                                        e1->y += (-k - y) >> 1;
                                    }
                                }

                                if (!e2->nocollision) {
                                    if (x > 0) {
                                        e2->x -= (k - x) >> 1;
                                    }

                                    if (x < 0) {
                                        e2->x -= (-k - x) >> 1;
                                    }

                                    if (y > 0) {
                                        e2->y -= (k - y) >> 1;
                                    }

                                    if (y < 0) {
                                        e2->y -= (-k - y) >> 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // These are some basic surface effects for common textures.
    yeti->surfaces.item[TEXTURE_SKY   ].xpan = (yeti->keyboard.tick << 2) - (yeti->camera->x >> 1);
    yeti->surfaces.item[TEXTURE_SKY   ].ypan = (yeti->keyboard.tick << 2) - (yeti->camera->y >> 1);
    yeti->surfaces.item[TEXTURE_LAVA  ].xpan = fixsin(yeti->keyboard.tick << 8) >> 2;
    yeti->surfaces.item[TEXTURE_LAVA  ].ypan = fixcos(yeti->keyboard.tick << 8) >> 2;
    yeti->surfaces.item[TEXTURE_WATER ].xpan = yeti->surfaces.item[TEXTURE_LAVA].xpan;
    yeti->surfaces.item[TEXTURE_WATER ].ypan = yeti->surfaces.item[TEXTURE_LAVA].ypan;
}

/******************************************************************************/



