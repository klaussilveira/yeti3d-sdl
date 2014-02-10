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

#include "y3d_matrix.h"

/*
** Name: matrix_rotate_world
** Desc: Calculates a world rotation matrix.
*/
void matrix_rotate_world(matrix_t m, int r, int p, int t)
{
    int cosalp = fixcos(r);
    int sinalp = fixsin(r);
    int cosbet = fixcos(p);
    int sinbet = fixsin(p);
    int cosgam = fixcos(t);
    int singam = fixsin(t);
    m[0][0] =  fixmul(singam, fixmul(sinbet, sinalp)) + fixmul(cosgam, cosalp);
    m[0][1] =  fixmul(cosbet, sinalp);
    m[0][2] =  fixmul(singam, cosalp) - fixmul(cosgam, fixmul(sinbet, sinalp));
    m[0][3] =  0;
    m[1][0] =  fixmul(singam, fixmul(sinbet, cosalp)) - fixmul(cosgam, sinalp);
    m[1][1] =  fixmul(cosbet, cosalp);
    m[1][2] = -fixmul(cosgam, fixmul(sinbet, cosalp)) - fixmul(singam, sinalp);
    m[1][3] =  0;
    m[2][0] = -fixmul(singam, cosbet);
    m[2][1] =  sinbet;
    m[2][2] =  fixmul(cosgam, cosbet);
    m[2][3] =  0;
}

/*
** Name: matrix_rotate_object
** Desc: Calculates a object rotation matrix.
*/
void matrix_rotate_object(matrix_t m, int p, int r, int t, int x, int y, int z)
{
    int cosalp = fixcos(p);
    int sinalp = fixsin(p);
    int cosbet = fixcos(r);
    int sinbet = fixsin(r);
    int cosgam = fixcos(t);
    int singam = fixsin(t);
    m[0][0] =  fixmul(cosalp, cosgam) - fixmul(sinalp, fixmul(sinbet, singam));
    m[0][1] =  fixmul(sinalp, cosgam) + fixmul(cosalp, fixmul(sinbet, singam));
    m[0][2] =  fixmul(cosbet, singam);
    m[0][3] =  x;
    m[1][0] = -fixmul(sinalp, cosbet);
    m[1][1] =  fixmul(cosalp, cosbet);
    m[1][2] = -sinbet;
    m[1][3] =  y;
    m[2][0] = -fixmul(cosalp, singam) - fixmul(sinalp, fixmul(sinbet, cosgam));
    m[2][1] =  fixmul(cosalp, fixmul(sinbet, cosgam)) - fixmul(sinalp, singam);
    m[2][2] =  fixmul(cosbet, cosgam);
    m[2][3] =  z;
}

/*
** Name: matrix_multiply
** Desc: Multiplies matrix (B) by (C) and places the result in (A).
** Note: Matrix (A) _must_ not be either (B) or (C).
*/
void matrix_multiply(matrix_t a, matrix_t b, matrix_t c)
{
    int i;

    for (i = 3; i--;) {
        a[i][0] = fixmul(c[i][0], b[0][0]) + fixmul(c[i][1], b[1][0]) + fixmul(c[i][2], b[2][0]);
        a[i][1] = fixmul(c[i][0], b[0][1]) + fixmul(c[i][1], b[1][1]) + fixmul(c[i][2], b[2][1]);
        a[i][2] = fixmul(c[i][0], b[0][2]) + fixmul(c[i][1], b[1][2]) + fixmul(c[i][2], b[2][2]);
        a[i][3] = fixmul(c[i][0], b[0][3]) + fixmul(c[i][1], b[1][3]) + fixmul(c[i][2], b[2][3]);
    }
}

