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

#include "y3d_file.h"

/******************************************************************************/

/*
** Name: file_load
** Desc: Allocates memory and loads a file into it. You must free the memory
**       after use.
*/
void *file_load(char *filename)
{
    void *mem = NULL;
    FILE *fp = fopen(filename, "rb");

    if (fp) {
        if (!fseek(fp, 0, SEEK_END)) {
            int n = ftell(fp);

            if ((mem = malloc(n)) != 0) {
                fseek(fp, 0, SEEK_SET);
                fread(mem, 1, n, fp);
            }
        }

        fclose(fp);
    }

    return mem;
}
