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

#include "y3d_record.h"

/*
** Name: record_write
** Desc:
*/
void record_write(record_t *record, keyboard_t *keyboard)
{
    if (record->file) {
        fwrite(&keyboard->state, 1, 1, record->file);
    }
}

/*
** Name: record_read
** Desc:
*/
void record_read(record_t *record, keyboard_t *keyboard)
{
    if (record->file) {
        fread(&keyboard->state, 1, 1, record->file);
    }
}

/*
** Name: record_start
** Desc:
*/
void record_start(record_t *record, char *filename)
{
    record->file = fopen(filename, "wb");
    record->isplaying = FALSE;
}

/*
** Name: record_rewind
** Desc:
*/
void record_rewind(record_t *record)
{
    if (record->file) {
        fseek(record->file, 0, SEEK_SET);
    }
}

/*
** Name: record_play
** Desc:
*/
void record_play(record_t *record, char *filename)
{
    record->file = fopen(filename, "rb");
    record->isplaying = TRUE;
}

/*
** Name: record_play_or_record
** Desc:
*/
void record_play_or_record(record_t *record, char *filename)
{
    record_play(record, filename);

    if (!record->file) {
        record_start(record, filename);
    }
}

/*
** Name: record_stop
** Desc:
*/
void record_stop(record_t *record)
{
    if (record->file) {
        fclose(record->file);
        record->file = NULL;
    }
}
