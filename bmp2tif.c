

   /************************************************
   *
   *       file bmp2tif.c
   *
   *       Functions: This file contains
   *           main
   *
   *       Purpose:
   *          This program creates a tiff file
   *          that is just like the input bmp file.
   *
   *       External Calls:
   *          imageio.c
   *             does_not_exist
   *             get_image_size
   *             read_image_array
   *             write_image_array
   *             free_image_array
   *             create_allocate_tif_file
   *
   *       Modifications:
   *         27 September 1998 - created
   *
   *************************************************/

#include "cips.h"


main(argc, argv)
   int  argc;
   char *argv[];
{
   char   *cc;
   int    l, w;
   int    ok = 0;
   short  **the_image;
   struct tiff_header_struct image_header;
   struct bmpfileheader      bmp_file_header;
   struct bitmapheader       bmheader;

   if(argc < 3 || argc > 3){
      printf(
      "\nusage: bmp2tif bmp-file-name tif-file-name\n");
      exit(-1);
   }

   if(does_not_exist(argv[1])){
    printf("\nERROR input file %s does not exist",
             argv[1]);
    exit(0);
   }

   cc = strstr(argv[1], ".bmp");
   if(cc == NULL){  
      printf("\nERROR %s must be a bmp file",
             argv[1]);
    exit(0);
   }  /* ends tif */

   cc = strstr(argv[2], ".tif");
   if(cc == NULL){  /* create a bmp */
      printf("\nERROR %s must be a tiff file name",
             argv[2]);
      exit(0);
   }

   get_image_size(argv[1], &l, &w);
   the_image = allocate_image_array(l, w);
   image_header.lsb            = 1;
   image_header.bits_per_pixel = 8;
   image_header.image_length   = l;
   image_header.image_width    = w;;
   image_header.strip_offset   = 1000;
   create_allocate_tiff_file(argv[2], 
                             &image_header);

   read_image_array(argv[1], the_image);
   write_image_array(argv[2], the_image);
   free_image_array(the_image, l);

}  /* ends main */
