
       /*********************************************
       *
       *   file mainover.c
       *
       *   Functions: This file contains
       *      main
       *
       *   Purpose:
       *      This file contains the main calling
       *      routine that calls the overlay functions.
       *
       *   External Calls:
       *      imageio.c - create_image_file
       *                  read_image_array
       *                  write_image_array
       *                  get_image_size
       *                  allocate_image_array
       *                  free_image_array
       *      overlay.c - non_zero_overlay
       *                  zero_overlay
       *                  greater_overlay
       *                  less_overlay
       *                  average_overlay
       *
       *   Modifications:
       *      6 March 1993 - created
       *      22 August 1998 - modified to work on
       *           entire images at once.
       *      19 September 1998 - modified to work with
       *           all I O routines in imageio.c.
       *
       ********************************************/

#include "cips.h"

int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();
int non_zero_overlay();
int are_not_same_size();
int zero_overlay();
int greater_overlay();
int less_overlay();
int average_overlay();


short **the_image;
short **out_image;

int main(argc, argv)
   int argc;
   char *argv[];
{

   char     name1[80], name2[80], name3[80], type[80];
   int      count, i, j;
   long     length, width;

       /*********************************************
       *
       *   Interpret the command line parameters.
       *
       **********************************************/

   if(argc < 5){
    printf(
    "\n\nNot enough parameters:"
     "\n"
     "\n usage: mainover in-file1 in-file2 out-file "
     "type"
     "\n"
     "\n recall type: nonzero zero greater less"
     " average"
     "\n the input images must be the same size"
     "\n"
     "\n");
    exit(0);
   }

   strcpy(name1,  argv[1]);
   strcpy(name2, argv[2]);
   strcpy(name3, argv[3]);
   strcpy(type,  argv[4]);

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

       /*********************************************
       *
       *   Read the input image headers.
       *   Ensure the input image are the same size.
       *   Allocate the image arrays and read
       *   the image data.
       *
       **********************************************/

   if(are_not_same_size(name1, name2)){
      printf(
       "\n Images %s and %s are not the same size",
       name1, name2);
      exit(1);
   }  /* ends if sizes not the same */

   get_image_size(name1, &length, &width);
   the_image = allocate_image_array(length, width);
   out_image = allocate_image_array(length, width);
   create_image_file(name1, name3);
   read_image_array(name1, the_image);
   read_image_array(name2, out_image);

       /*********************************************
       *
       *   Apply the desired overlay function.
       *
       **********************************************/

            /* non-zero */
   if(strncmp("non", type, 3) == 0){
      non_zero_overlay(the_image, out_image,
                       length,
                       width);
   }  /* ends non_zero operation */

      /* zero */
   if(strcmp("zero", type) == 0){
      zero_overlay(the_image, out_image,
                   length,
                   width);
   }  /* ends zero operation */

      /* greater */
   if(strncmp("gre", type, 3) == 0){
      greater_overlay(the_image, out_image,
                      length,
                      width);
   }  /* ends greater operation */

      /* less */
   if(strncmp("les", type, 3) == 0){
      less_overlay(the_image, out_image,
                   length,
                   width);
   }  /* ends less operation */

      /* average */
   if(strncmp("ave", type, 3) == 0){
      average_overlay(the_image, out_image,
                      length,
                      width);
   }  /* ends average operation */

   write_image_array(name3, out_image);
   free_image_array(out_image, length);
   free_image_array(the_image, length);

}  /* ends main  */
