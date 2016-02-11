
   /***********************************************
   *
   *   file dumpi.c
   *
   *   Functions: This file contains
   *      main
   *
   *   Purpose:
   *      This file contains a program that 
   *      is very similar to dumpi.  Dumpi dumps 
   *      the number values of an image
   *      to an ascii text file.
   *      This program sends a space to a text
   *      file for zeros in the image and an
   *      asterisk for non-zeros in the image.
   *
   *   External Calls:
   *      imageio.c - read_image_array
   *                  get_image_size
   *                  allocate_image_array
   *                  free_image_array
   *                  does_not_exist
   *
   *   Modifications:
   *      3 October 1998 - created to work with 
   *           all I O routines in imageio.c.
   *
   *************************************************/

#include "cips.h"

main(argc, argv)
   int argc;
   char *argv[];
{
   char  in_name[MAX_NAME_LENGTH];
   char  out_name[MAX_NAME_LENGTH];
   char  *line, buffer[10];
   int   i, j;
   long  height, width;
   short **the_image;
   FILE  *out_file;

   
      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc != 3){
    printf("\nusage: dumpb input-image output-file");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   strcpy(out_name, argv[2]);
   
      /******************************************
      *
      *   Ensure the input image exists.
      *   Read the input image and dump the
      *   1s and 0s to an output text file.
      *
      ******************************************/

   if(does_not_exist(in_name)){
      printf("\nERROR input file %s does not exist",
             in_name);
      exit(0);
   }  /* ends if does_not_exist */

   if((out_file = fopen(out_name, "wt")) == NULL){
      printf("\nERROR Could not open file %s",
             out_name);
      exit(2);
   }


   get_image_size(in_name, &height, &width);
   the_image = allocate_image_array(height, width);
   read_image_array(in_name, the_image);

   line = malloc( (width+7) * sizeof(char  *));

   sprintf(line, "      ");
   for(i=0; i<width; i++){
      sprintf(buffer, "%4d", i);
      strcat(line, buffer);
   }
   strcat(line, "\n");
   fputs(line, out_file);

   for(i=0; i<height; i++){
      sprintf(line, "%5d>", i);
      for(j=0; j<width; j++){
         if(the_image[i][j] == 0)
            strcat(line, " ");
         else
            strcat(line, "*");
      }
      strcat(line, "\n");
      fputs(line, out_file);
   }

   free_image_array(the_image, height);
   fclose(out_file);

}  /* ends main */
