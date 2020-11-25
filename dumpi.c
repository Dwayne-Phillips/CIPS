
   /***********************************************
   *
   *   file dumpi.c
   *
   *   Functions: This file contains
   *      main
   *
   *   Purpose:
   *      This file contains a program that
   *      dumps the number values of an image
   *      to an ascii text file.
   *
   *   External Calls:
   *      imageio.c - get_image_size
   *                  read_image_array
   *                  allocate_image_array
   *                  free_image_array
   *
   *   Modifications:
   *      1 October 1998 - created to work with
   *           all I O routines in imageio.c.
   *
   *************************************************/

#include "cips.h"

int does_not_exist();
int get_image_size();
int read_image_array();
int free_image_array();


int main(argc, argv)
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
    printf("\nusage: dumpi input-image output-file");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   strcpy(out_name, argv[2]);

      /******************************************
      *
      *   Ensure the input image exists.
      *   Create the output text file.
      *   Allocate an image array.
      *   Read the image and dump the nubmers
      *   to a text file.
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

   line = malloc( ((width*4)+7) * sizeof(char  *));

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
         sprintf(buffer, "-%3d", the_image[i][j]);
         strcat(line, buffer);
      }
      strcat(line, "\n");
      fputs(line, out_file);
   }

   free_image_array(the_image, height);
   fclose(out_file);

}  /* ends main */
