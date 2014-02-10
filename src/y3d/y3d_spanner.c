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

#include "y3d_spanner.h"

/*
** Name: spanarray_init
** Desc:
*/
void spanarray_init(spanarray_t *dst, int x1, int x2)
{
    dst->nspans = 1;
    dst->spans[0].x1 = x1;
    dst->spans[0].x2 = x2;
}

/*
** Name: spanarray_validate
** Desc:
*/
void spanarray_validate(spanarray_t *array, int x1, int x2)
{
    int n;
    span_t *span    = array->spans;
    span_t *newspan = &array->spans[array->nspans];

    for (n = array->nspans; n--; span++) {
        if (x1 < span->x2 && x2 > span->x1) {
            if (x1 > span->x1) {
                if (x2 < span->x2) {
                    newspan->x1 = x2;
                    newspan->x2 = span->x2;
                    newspan++;
                    array->nspans++;
                }

                span->x2 = x1;
            } else {
                if (x2 < span->x2) {
                    span->x1 = x2;
                } else {
                    span_t *s;

                    for (s = span; s != newspan; s[0] = s[1], s++);

                    newspan--;
                    span--;
                    array->nspans--;
                }
            }
        }
    }

    assert(array->nspans >= 0 && array->nspans < 60);
}

/*
** Name: spanner_invalidate
** Desc:
*/
void spanner_invalidate(spanner_t *spanner)
{
    int i;
    spanner->ngaps = spanner->height;

    for (i = 0; i < spanner->height; i++) {
        spanarray_init(&spanner->lines[i], 0, spanner->width);
    }
}

/*
** Name: spanner_validate
** Desc:
*/
void spanner_validate(spanner_t *spanner, int x1, int y1, int x2, int y2)
{
    int i;

    for (i = y1; i < y2; i++) {
        spanarray_validate(&spanner->lines[i], x1, x2);

        if (spanner->lines[i].nspans == 0) {
            spanner->ngaps--;
        }
    }
}

/*
void SpannerCreate(spanner_t* spanner, int width, int height)
{
  memset(spanner, 0, sizeof(spanner_t));

  spanner->width  = width;
  spanner->height = height;
  spanner->lines  = (spanarray_t*) calloc(height, sizeof(spanarray_t));
}

void SpannerDelete(spanner_t* spanner)
{
  free(spanner->lines);
}
*/

