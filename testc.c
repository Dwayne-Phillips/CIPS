
/*******************

November 2020
This was a test stuff. It is no
longer needed.

******************/

   /************************************************
   *
   *       file d:\cips\create.c
   *
   *       Functions: This file contains
   *           main
   *
   *       Purpose:
   *          This program creates an 8 bit tiff file
   *          of size l*ROWS by w*COLS.
   *
   *       External Calls:
   *          imageio.c
   *             create_allocate_tif_file
   *             create_allocate_bmp_file
   *
   *       Modifications:
   *          7 Arpil 1992 - created
   *         15 August 1998 - modified to work with
   *              an entire image array at once.
   *         18 September 1998 - modified to work with
   *              all I O routines in imageio.c.
   *
   *************************************************/

#include "cips.h"
#include <conio.h>


main(argc, argv)
   int  argc;
   char *argv[];
{
   char   x, y, z;
   char   *cc;
   int    l, w;
   int    ok = 0;
   int    a, b;
   struct tiff_header_struct image_header;
   struct bmpfileheader      bmp_file_header;
   struct bitmapheader       bmheader;

   a = getch();
   printf("\n-%x-", a);


}
