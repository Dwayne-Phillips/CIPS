


       /***********************************************
       *
       *   file mfilter.c
       *
       *   Functions: This file contains
       *      main
       *
       *   Purpose:
       *      This is the main calling program for
       *      a set of spatial filtering routines.
       *
       *   External Calls:
       *      imageio.c - create_image_file
       *                  read_image_array
       *                  write_image_array
       *                  get_image_size
       *                  get_bitsperpixel
       *                  allocate_image_array
       *                  free_image_array
       *      filter.c -  filter_image
       *                  median_filter
       *                  high_pixel
       *                  low_pixel
       *
       *   Modifications:
       *      15 February 1992 - created
       *      01 January 1993 - added calls to
       *          high_pixel and low_pixel.
       *      18 September 1998 - modified to work with 
       *           all I O routines in imageio.c.
       *
       ***********************************************/



#include "cips.h"


main(argc, argv)
   int argc;
   char *argv[];
{
   char     name1[MAX_NAME_LENGTH];
   char     name2[MAX_NAME_LENGTH];
   char     low_high[MAX_NAME_LENGTH];
   int      i, j, size, type;
   long     bits_per_pixel, length, width;
   short    **the_image, **out_image, filter[3][3];

   
      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc < 5  ||  argc > 6){
    printf(
    "\nusage 1: mfilter in-image out-image g Low-High type]"
    "\n                 OR"
    "\nusage 2: mfilter in-image out-image High-Low-Median size"
    "\n                   h - high pixel"
    "\n                   l - low pixel"
    "\n                   m - median pixel"
    "\n                  "
    "\n Using the General type requires entering "
    "\n the type per the following table"
    "\n L -  6"
    "\n L -  9"
    "\n L - 10"
    "\n L - 16"
    "\n L - 32"
    "\n H -  1"
    "\n H -  2"
    "\n H -  3"
    "\n Using the High-Low-Median type requirs entering "
    "\n the size of the filtered area 3 (3x3) 5, 7, 9, etc.");
    exit(0);
   }

   strcpy(name1, argv[1]);
   strcpy(name2, argv[2]);

   if(does_not_exist(name1)){
      printf("\nERROR input file %s does not exist",
             name1);
      exit(0);
   }  /* ends if does_not_exist */
   
      /******************************************
      *
      *   Read the input image header, allocate
      *   the image arrays, create the output
      *   image, and read the input image.
      *
      ******************************************/

   create_image_file(name1, name2);
   get_image_size(name1, &length, &width);
   get_bitsperpixel(name1, &bits_per_pixel);
   the_image = allocate_image_array(length, width);
   out_image = allocate_image_array(length, width);
   read_image_array(name1, the_image);
   
      /******************************************
      *
      *   Call the proper image filter function
      *   per the command line.
      *
      ******************************************/


     /* General filtering case */
   if(argc == 6){  
      strcpy(low_high, argv[4]);
      type = atoi(argv[5]);
      filter_image(the_image, out_image,
                   length,
                   width,
                   bits_per_pixel,
                   filter, type, low_high);
   }  /* ends if argc == 6 */


     /* High, Low, and Median filtering cases */
   if(argc == 5){  
      strcpy(low_high, argv[3]);
      size = atoi(argv[4]);
      if(low_high[0] == 'h' || low_high[0] == 'H')
         high_pixel(the_image, out_image,
                    length,
                    width,
                    size); 
      if(low_high[0] == 'l' || low_high[0] == 'L')
         low_pixel(the_image, out_image,
                   length,
                   width,
                   size); 
      if(low_high[0] == 'm' || low_high[0] == 'M')
         median_filter(the_image, out_image,
                       length,
                       width,
                       size); 
   }  /* ends if argc == 5 */

      /******************************************
      *
      *   Write the output image and free the
      *   image arrays: THE END
      *
      ******************************************/

   write_image_array(name2, out_image);
   free_image_array(the_image, length);
   free_image_array(out_image, length);

}  /* ends main */
