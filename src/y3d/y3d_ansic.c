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

#include "y3d_ansic.h"
#include "y3d_fixed.h"

/******************************************************************************/

/*
** Name: ansic_stpcpy
** Desc: A Win32 function that should be in ansic.
*/
char *ansic_stpcpy(char *dst, char *src)
{
    while ((*dst++ = *src++) != 0);

    return dst - 1;
}

/*
** Name: ansic_itos
** Desc:
*/
void ansic_itos(int i, char *s)
{
    //#define DIGITS(X) if(i>=X){c=0;do{i-=X;c++;}while(i >= X);*s++='0'+c;}
#define DIGITS(X) for(c=0;i>=X;i-=X,c++);*s++='0'+c;
    int c;
    //DIGITS(100000);
    //DIGITS( 10000);
    //DIGITS(  1000);
    DIGITS(100);
    DIGITS(10);
    *s++ = '0' + i;
    *s = 0;
}

/*
** Name: ansic_friction
** Desc:
*/
int ansic_friction(int a, int b)
{
    return a - f2i((b = a * b) < 0 ? b - FIXCEIL : b + FIXCEIL);
}

/*
** Name: isqrt
** Desc: Integer square root. Take the square root of an integer.
** Note: Currently unused. Needs testing.
*/
int isqrt(int value)
{
    int root = 0;
#define STEP(shift) \
    if((0x40000000 >> shift) + root <= value)          \
    {                                                   \
        value -= (0x40000000 >> shift) + root;          \
        root = (root >> 1) | (0x40000000 >> shift);     \
    }                                                   \
    else                                                \
    {                                                   \
        root >>= 1;                                     \
    }
    STEP(0);
    STEP(2);
    STEP(4);
    STEP(6);
    STEP(8);
    STEP(10);
    STEP(12);
    STEP(14);
    STEP(16);
    STEP(18);
    STEP(20);
    STEP(22);
    STEP(24);
    STEP(26);
    STEP(28);
    STEP(30);

    // round to the nearest integer, cuts max error in half

    if (root < value) {
        root++;
    }

    return root;
}


