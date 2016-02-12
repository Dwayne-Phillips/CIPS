
   /***********************************************
   *
   *   file invert.c
   *
   *   Functions: This file contains
   *      main
   *
   *   Purpose:
   *      This program takes an image file and
   *      inverts it. It works with 8 bit images
   *      only.
   *
   *   External Calls:
   *          imageio.c
   *             create_allocate_tif_file
   *             create_allocate_bmp_file
   *             get_image_size
   *             allocate_image_array
   *             free_image_array
   *             read_image_array
   *             write_image_array
   *
   *   Modifications:
   *      6 March 1993 - created
   *      22 August 1998 - modified to work on entire
   *           images at once.
   *      19 September 1998 - modified to work with 
   *           all I O routines in imageio.c.
   *
   ***********************************************/

#include "cips.h"




main(argc, argv)
   int argc;
   char *argv[];
{
   char     name1[80], name2[80];
   char     *cc;
   int      a, b; 
   int      ok = 0;
   long     length, width;
   short    **the_image;
   struct tiff_header_struct image_header;
   struct bmpfileheader      bmp_file_header;
   struct bitmapheader       bmheader;

   if(argc != 3){
      printf("\nusage: invert in-file out-file\n");
      exit(1);
   }

   strcpy(name1, argv[1]);
   strcpy(name2, argv[2]);

   get_image_size(name1, &length, &width);
  
   the_image = allocate_image_array(length, width);

   cc = strstr(argv[1], ".tif");
   if(cc != NULL){  /* create a tif */
      ok = 1;
      image_header.lsb            = 1;
      image_header.bits_per_pixel = 8;
      image_header.image_length   = length;
      image_header.image_width    = width;
      image_header.strip_offset   = 1000;
      create_allocate_tiff_file(argv[2], 
                                &image_header);
   }  /* ends tif */

   cc = strstr(argv[1], ".bmp");
   if(cc != NULL){  /* create a bmp */
      ok = 1;
      bmheader.height = length;
      bmheader.width  = width;
      create_allocate_bmp_file(argv[2], 
                               &bmp_file_header, 
                               &bmheader);
   }  /* ends tif */

   if(ok == 0){
      printf("\nERROR input file neither tiff nor bmp");
      exit(0);
   }

   read_image_array(name1, the_image);

   for(a=0; a<length; a++)
      for(b=0; b<width; b++)
         the_image[a][b] = GRAY_LEVELS-the_image[a][b];

   write_image_array(name2, the_image);

   free_image_array(the_image, length);

}  /* ends main */
