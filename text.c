


    /************************************************
    *
    *   file d:\cips\text.c
    *
    *   Functions: This file contains
    *       main
    *       overlay_text
    *
    *   Purpose:
    *      This program writes simple block letters
    *      the an image file.  You can use these
    *      as labels for other images.
    *
    *   External Calls:
    *      wtiff.c - create_allocate_tiff_file
    *                read_tiff_image
    *
    *   Modifications:
    *      16 February 1998 - created
    *
    *************************************************/


#include "cips.h"

#define R             9
#define C             7
#define COUNTER_LIMIT 8
#define IE_START      7
#define VAL         200

int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();
int create_allocate_tiff_file();
int read_tiff_image();
int write_array_into_tiff_image();
int copy_array_into_image();


short image[ROWS][COLS];

         /******************************
         *
         *   Define all the 9x7 arrays
         *   that contain the characters.
         *
         *******************************/

short aperiod[R][C] = {
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,  0,  0,  0},
                   {  0,  0,VAL,VAL,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short acomma[R][C] = {
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,  0,  0,  0},
                   {  0,  0,VAL,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0}};

short aexclam[R][C] = {
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short xx[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short aa[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,VAL,  0,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ab[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ac[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ad[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ae[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short af[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ag[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,VAL,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ah[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ai[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short aj[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,VAL,  0,VAL,  0,  0,  0},
                   {  0,VAL,  0,VAL,  0,  0,  0},
                   {  0,  0,VAL,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ak[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,VAL,  0,  0},
                   {  0,VAL,  0,VAL,  0,  0,  0},
                   {  0,VAL,VAL,  0,  0,  0,  0},
                   {  0,VAL,  0,VAL,  0,  0,  0},
                   {  0,VAL,  0,  0,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short al[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short am[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,VAL,  0,VAL,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short an[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,VAL,  0,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,  0,  0,VAL,VAL,  0},
                   {  0,VAL,  0,  0,VAL,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ao[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ap[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short aq[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,  0,  0,VAL,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ar[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};
short as[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short at[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short au[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short av[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,  0,VAL,  0,  0},
                   {  0,  0,VAL,  0,VAL,  0,  0},
                   {  0,  0,VAL,  0,VAL,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short aw[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,VAL,  0,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ax[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,  0,VAL,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,VAL,  0,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short ay[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,  0,VAL,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short az[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,VAL,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,VAL,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short a1[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,VAL,VAL,  0,  0,  0},
                   {  0,VAL,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short a2[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,VAL,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,VAL,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short a3[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short a4[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,VAL,  0,  0,  0},
                   {  0,VAL,  0,VAL,  0,  0,  0},
                   {  0,VAL,  0,VAL,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short a5[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short a6[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short a7[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,VAL,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,VAL,  0,  0},
                   {  0,  0,  0,VAL,  0,  0,  0},
                   {  0,  0,VAL,  0,  0,  0,  0},
                   {  0,VAL,  0,  0,  0,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short a8[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short a9[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,VAL,  0,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,VAL,  0},
                   {  0,  0,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,VAL,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};

short a0[R][C] = { {  0,  0,  0,  0,  0,  0,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,VAL,  0,  0,VAL,VAL,  0},
                   {  0,VAL,  0,  0,VAL,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,  0,VAL,  0,VAL,  0},
                   {  0,VAL,VAL,  0,  0,VAL,  0},
                   {  0,  0,VAL,VAL,VAL,  0,  0},
                   {  0,  0,  0,  0,  0,  0,  0}};



int main(argc, argv)
   int  argc;
   char *argv[];
{
   int    l, w;
   int    counter=0, i, j, il, ie=7, ll, le;
   int    lsave, esave;
   struct tiff_header_struct image_header;


   if(argc < 6){
      printf("\n usage: ilabel file-name il ie l w text");
      printf("\n        where out image will be lxROWS wxCOLS\n");
      exit(-1);
   }

         /******************************
         *
         *   Setup the output file.
         *
         *******************************/

   image_header.lsb            = 1;
   image_header.bits_per_pixel = 8;
   image_header.image_length   = atoi(argv[4])*ROWS;
   image_header.image_width    = atoi(argv[5])*COLS;;
   image_header.strip_offset   = 1000;

   if(does_not_exist(argv[1]))
      create_allocate_tiff_file(argv[1],
                                &image_header,
                                image);
   else
      printf("\n%s already exists", argv[1]);

   il = atoi(argv[2]);
   ie = atoi(argv[3]);

   read_tiff_image(argv[1], image, il-R, ie-C,
                   il-R+ROWS, ie-C+COLS);

printf("\nDEBUG> reading from %d %d",il-R, ie-C);

   lsave = il;
   esave = ie;

   while(il >= ROWS)
      il = il - ROWS;
   while(ie >= COLS)
      ie = ie - COLS;

printf("\nDEBUG> writing into %d %d", il, ie);

         /******************************
         *
         *   Loop through the text
         *   arguments and place the
         *   letter arrays into the
         *   image.
         *
         *******************************/

   printf("\n");
   for(i=6; i<argc; i++){
      for(j=0; j<(strlen(argv[i])); j++){

         printf("%c", argv[i][j]);
         if(argv[i][j] == 'a')
            copy_array_into_image(aa, image, il, ie);
         if(argv[i][j] == 'b')
            copy_array_into_image(ab, image, il, ie);
         if(argv[i][j] == 'c')
            copy_array_into_image(ac, image, il, ie);
         if(argv[i][j] == 'd')
            copy_array_into_image(ad, image, il, ie);
         if(argv[i][j] == 'e')
            copy_array_into_image(ae, image, il, ie);
         if(argv[i][j] == 'f')
            copy_array_into_image(af, image, il, ie);
         if(argv[i][j] == 'g')
            copy_array_into_image(ag, image, il, ie);
         if(argv[i][j] == 'h')
            copy_array_into_image(ah, image, il, ie);
         if(argv[i][j] == 'i')
            copy_array_into_image(ai, image, il, ie);
         if(argv[i][j] == 'j')
            copy_array_into_image(aj, image, il, ie);
         if(argv[i][j] == 'k')
            copy_array_into_image(ak, image, il, ie);
         if(argv[i][j] == 'l')
            copy_array_into_image(al, image, il, ie);
         if(argv[i][j] == 'm')
            copy_array_into_image(am, image, il, ie);
         if(argv[i][j] == 'n')
            copy_array_into_image(an, image, il, ie);
         if(argv[i][j] == 'o')
            copy_array_into_image(ao, image, il, ie);
         if(argv[i][j] == 'p')
            copy_array_into_image(ap, image, il, ie);
         if(argv[i][j] == 'q')
            copy_array_into_image(aq, image, il, ie);
         if(argv[i][j] == 'r')
            copy_array_into_image(ar, image, il, ie);
         if(argv[i][j] == 's')
            copy_array_into_image(as, image, il, ie);
         if(argv[i][j] == 't')
            copy_array_into_image(at, image, il, ie);
         if(argv[i][j] == 'u')
            copy_array_into_image(au, image, il, ie);
         if(argv[i][j] == 'v')
            copy_array_into_image(av, image, il, ie);
         if(argv[i][j] == 'w')
            copy_array_into_image(aw, image, il, ie);
         if(argv[i][j] == 'x')
            copy_array_into_image(ax, image, il, ie);
         if(argv[i][j] == 'y')
            copy_array_into_image(ay, image, il, ie);
         if(argv[i][j] == 'z')
            copy_array_into_image(az, image, il, ie);
         if(argv[i][j] == '1')
            copy_array_into_image(a1, image, il, ie);
         if(argv[i][j] == '2')
            copy_array_into_image(a2, image, il, ie);
         if(argv[i][j] == '3')
            copy_array_into_image(a3, image, il, ie);
         if(argv[i][j] == '4')
            copy_array_into_image(a4, image, il, ie);
         if(argv[i][j] == '5')
            copy_array_into_image(a5, image, il, ie);
         if(argv[i][j] == '6')
            copy_array_into_image(a6, image, il, ie);
         if(argv[i][j] == '7')
            copy_array_into_image(a7, image, il, ie);
         if(argv[i][j] == '8')
            copy_array_into_image(a8, image, il, ie);
         if(argv[i][j] == '9')
            copy_array_into_image(a9, image, il, ie);
         if(argv[i][j] == '0')
            copy_array_into_image(a0, image, il, ie);
         if(argv[i][j] == '.')
            copy_array_into_image(aperiod,image,il,ie);
         if(argv[i][j] == ',')
            copy_array_into_image(acomma,image,il,ie);
         if(argv[i][j] == '!')
            copy_array_into_image(aexclam,image,il,ie);


         ie = ie + C;
         counter++;
         if(counter > COUNTER_LIMIT){
            ie      = IE_START;
            il      = il+R+1;
            counter = 0;
         }
      }  /* ends loop over letters in argument */
         copy_array_into_image(xx, image, il, ie);
         ie = ie + C;
         counter++;
         if(counter > COUNTER_LIMIT){
            ie      = IE_START;
            il      = il+R+1;
            counter = 0;
         }
   }  /* ends loop over arguments */

   il = lsave;
   ie = esave;
   write_array_into_tiff_image(argv[1], image,
                               il, ie,
                               il+ROWS, ie+COLS);
printf("\nDEBUG> Writing output file %d %d", il, ie);
}


int copy_array_into_image(a, the_image, il, ie)
   short a[R][C], the_image[ROWS][COLS];
   int   il, ie;
{
   int i, j;
   for(i=0; i<R; i++)
      for(j=0; j<C; j++)
         the_image[il+i][ie+j] = a[i][j];

return(1);
}  /* ends copy_array_into_image */
