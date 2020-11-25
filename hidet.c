

    /***********************************************
    *
    *  file hidet.c
    *
    *  Functions: This file contains
    *     main
    *
    *  Purpose:
    *     This file contains the main calling
    *     routine and subroutines to overlay
    *     text on top of an image.
    *
    *  External Calls:
    *      imageio.c - create_image_file
    *                  read_image_array
    *                  write_image_array
    *                  get_image_size
    *                  allocate_image_array
    *                  free_image_array
    *
    *  Modifications:
    *     16 February 1998 - created
    *     22 September 1998 - modified to work with
    *           all I O routines in imageio.c.
    *
    *************************************************/

#include "cips.h"

int does_not_exist();
int are_not_same_size();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();


int main(argc, argv)
   int argc;
   char *argv[];
{

   char  image_name[80], water_name[80];
   int   i, j;
   long  length, width;
   short factor;
   short **the_image;
   short **out_image;

   if(argc < 4){
    printf("\n\nNot enough parameters:");
    printf("\n");
    printf("\n   usage: hidet image-file text-file-image factor  ");
    exit(0);
   }

   strcpy(image_name, argv[1]);
   strcpy(water_name, argv[2]);
   factor = atoi(argv[3]);

   if(does_not_exist(image_name)){
      printf("\nERROR input file %s does not exist",
             image_name);
      exit(0);
   }

   if(does_not_exist(water_name)){
      printf("\nERROR output file %s does not exist",
             water_name);
      exit(0);
   }

   if(are_not_same_size(image_name, water_name)){
      printf("\nERROR images are not same size");
      exit(0);
   }

   get_image_size(image_name, &length, &width);

   the_image = allocate_image_array(length, width);
   out_image = allocate_image_array(length, width);

   read_image_array(image_name, the_image);
   read_image_array(water_name, out_image);

   for(i=0; i<length; i++){
      for(j=0; j<width; j++){
         if(out_image[i][j] != 0){
            the_image[i][j] = the_image[i][j] + factor;
            if(the_image[i][j] > GRAY_LEVELS)
               the_image[i][j] = GRAY_LEVELS;
         }  /* ends if */
      } /* ends loop over j */
   }  /* ends loop over i */

   write_image_array(image_name, the_image);

   free_image_array(the_image, length);
   free_image_array(out_image, length);

}  /* ends main  */
