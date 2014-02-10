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

#include "y3d_fixed.h"

/******************************************************************************/

int sintable[YETI_SINTABLE_MAX];
int reciprocal[YETI_RECIPROCAL_MAX];

/******************************************************************************/

/*
** Name: lupin_arctan2
** Desc: Returns an approximate arctan2 angle. Angles are
**       0..2048 = 0..360 degrees.
*/
int lupin_arctan2(int y, int x)
{
    if (x && y) {
        int a = ABS(x);
        y = y < 0 ? 1792 - fixdiv(y + a, a - y) : 1280 - fixdiv(y - a, y + a);
        return x < 0 ? -y : y;
    }

    return 0;
}

u16 fixangle(int x, int y)
{
    return lupin_arctan2(-y, -x) << 5;
}

/*
** Name: fixangle
** Desc: Returns the angle of a line. Angles are defined as
**       (u16)(0...65536) = 0..360 degrees. Which means they fit into u16's
**       and wrap nicely without using &'s. Plus you have ample number of
**       bits for angular velocity.
**       X & Y position uses 24:8 fixed point maths.
*/
/*
u16 _fixangle(int x, int y)
{
  static int table[32768];

  if (!table[100])
  {
    int i;

    for (i = 32768; i--;)
    {
      table[i] = atan(i * M_PI / 180.0) * 256.0 * 8192.0 / 360.0;
    }
  }
  if (y || x)
  {
    int yabs = ABS(y), xabs = ABS(x);
    int degs = xabs <= yabs ? table[fixdiv(xabs, yabs)] : DEG_90 - table[fixdiv(yabs, xabs)];
    return x < 0 ? y < 0 ? degs - DEG_180 : -degs : y < 0 ? DEG_180 - degs : degs;
  }
  else
  {
    return 0;
  }
}
*/



