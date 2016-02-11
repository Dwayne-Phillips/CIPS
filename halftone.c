
   /***********************************************
   *
   *   file halftone.c
   *
   *   Functions: This file contains
   *      main
   *
   *   Purpose:
   *      This file contains the main calling
   *      routine that performs histogram 
   *      equalization.
   *
   *   External Calls:
   *      imageio.c - create_image_file
   *                  read_image_array
   *                  write_image_array
   *                  get_image_size
   *                  allocate_image_array
   *                  free_image_array
   *                  does_not_exist
   *      ht.c - half_tone
   *
   *   Modifications:
   *     30 September 1998 - created to work with 
   *         all I O routines in imageio.c.
   *
   *************************************************/

#include "cips.h"

main(argc, argv)
   int argc;
   char *argv[];
{
   char  in_name[MAX_NAME_LENGTH];
   char  out_name[MAX_NAME_LENGTH];
   int   i;
   long  height, width;
   short **the_image, **out_image;
   short threshold;

   
      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc != 4){
    printf(
    "\nusage: halftone input-image output-image threshold");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   strcpy(out_name, argv[2]);
   threshold = atoi(argv[3]);
   
      /******************************************
      *
      *   Ensure the input image exists.
      *   Create the output image file.
      *   Allocate an image array, read the input 
      *   image, half_tone it, and write
      *   the result.
      *
      ******************************************/

   if(does_not_exist(in_name)){
      printf("\nERROR input file %s does not exist",
             in_name);
      printf("\n      "
             "usage: histeq input-image output-image");
      exit(0);
   }  /* ends if does_not_exist */

   create_image_file(in_name, out_name);
   get_image_size(in_name, &height, &width);
   the_image = allocate_image_array(height, width);
   out_image = allocate_image_array(height, width);
   read_image_array(in_name, the_image);

   half_tone(the_image, out_image,
             threshold, 200, 0,
             height, width);

   write_image_array(out_name, out_image);
   free_image_array(the_image, height);
   free_image_array(out_image, height);

}  /* ends main */
