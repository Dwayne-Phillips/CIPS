

   /***********************************************
   *
   *   file histeq.c
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
   *      hist.c - calculate_histogram
   *               perform_histogram_equalization
   *
   *   Modifications:
   *     18 September 1998 - created to work with
   *           all I O routines in imageio.c.
   *
   *************************************************/

#include "cips.h"

int calculate_histogram();
int perform_histogram_equalization();
int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();


int main(argc, argv)
   int argc;
   char *argv[];
{
   char  in_name[MAX_NAME_LENGTH];
   char  out_name[MAX_NAME_LENGTH];
   char  response[MAX_NAME_LENGTH];
   int   i;
   long  height, width;
   short **the_image;
   unsigned long histogram[GRAY_LEVELS+1];


      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc < 3){
    printf("\n   usage: histeq input-image output-image");
    printf("\n\n");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   strcpy(out_name, argv[2]);

      /******************************************
      *
      *   Ensure the input image exists.
      *   Create the output image file.
      *   Allocate an image array and call the
      *   histogram operators.
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
   read_image_array(in_name, the_image);

   for(i=0; i<GRAY_LEVELS+1; i++) histogram[i] = 0;

   calculate_histogram(the_image, histogram,
                       height, width);

   perform_histogram_equalization(
                       the_image, histogram,
                       256, 250,
                       height, width);

   write_image_array(out_name, the_image);
   free_image_array(the_image, height);

}  /* ends main */
