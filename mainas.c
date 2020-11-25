
   /***********************************************
   *
   *   file mainas.c
   *
   *   Functions: This file contains
   *      main
   *
   *   Purpose:
   *      This file contains the main calling
   *      routine in an image addition and subtraction
   *      program.
   *
   *   External Calls:
   *      imageio.c - create_image_file
   *                  read_image_array
   *                  write_image_array
   *                  get_image_size
   *                  get_bitsperpixel
   *                  allocate_image_array
   *                  free_image_array
   *                  does_not_exist
   *                  are_not_same_size
   *      baddsub.c - add_image_array
   *                  subtract_image_array
   *
   *   Modifications:
   *      1 April 1992 - created
   *     10 August 1998 - modified to work on entire
   *         images at one time.
   *     18 September 1998 - modified to work with
   *           all I O routines in imageio.c.
   *
   *************************************************/

#include "cips.h"

int does_not_exist();
int are_not_same_size();
int get_image_size();
int get_bitsperpixel();
int create_image_file();
int read_image_array();
int add_image_array();
int subtract_image_array();
int write_image_array();
int free_image_array();

int main(argc, argv)
   int  argc;
   char *argv[];
{

   char     name1[80], name2[80], name3[80];
   long     bits_per_pixel, length, width;
   short    **image1, **image2;
   short    max;

       /******************************************
       *
       *  Interpret the command line parameters.
       *
       *******************************************/

   if(argc != 5){
    printf(
     "\n"
     "\n usage: mainas in1-file in2-file "
     "out_file add-subtract"
     "\n"
     "\n   recall add-subtract a=add s=subtract\n");
    exit(0);
   }

   strcpy(name1, argv[1]);
   strcpy(name2, argv[2]);
   strcpy(name3, argv[3]);

   if(does_not_exist(name1)){
    printf("\nERROR input file %s does not exist",
             name1);
    exit(0);
   }

   if(does_not_exist(name2)){
    printf("\nERROR input file %s does not exist",
             name2);
    exit(0);
   }

       /******************************************
       *
       *  Ensure the two input images have the
       *  same sizes.
       *
       *******************************************/

   if(are_not_same_size(name1, name2)){
      printf(
      "\nERROR Image files %s and %s are not same size",
      name1, name2);
      exit(0);
   }

       /******************************************
       *
       *  Allocate the two image arrays
       *
       *******************************************/

   get_image_size(name1, &length, &width);
   get_bitsperpixel(name1, &bits_per_pixel);
   image1 = allocate_image_array(length, width);
   image2 = allocate_image_array(length, width);

       /******************************************
       *
       *  Create the output file and read the
       *  two input images.
       *
       *******************************************/

   create_image_file(name1, name3);
   read_image_array(name1, image1);
   read_image_array(name2, image2);

       /********************************************
       *
       *   Add or subtract the input images and
       *   write the result to the output image.
       *
       ********************************************/

   if(argv[4][0] == 'a' || argv[4][0] == 'A'){
      if(bits_per_pixel == 4)
         max = 16;
      else
         max = 255;
      add_image_array(image1, image2,
         length, width, max);
   }  /* ends if add */

   if(argv[4][0] == 's' || argv[4][0] == 'S')
      subtract_image_array(image1, image2,
         length, width);

   write_image_array(name3, image2);

   free_image_array(image1, length);
   free_image_array(image2, length);

}  /* ends main  */
