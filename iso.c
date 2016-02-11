
    /*********************************************
    *
    *       file iso.c
    *
    *       Functions: This file contains
    *          main
    *          lineup
    *
    *       Purpose:
    *          This program performs a simple
    *          form of isometric transform
    *          where the vertical is angled.
    *
    *       External Calls:
    *          imageio.c 
    *             create_resized_image_file
    *             read_image_array
    *             write_image_array
    *             get_image_size
    *             allocate_image_array
    *             free_image_array
    *
    *       Modifications:
    *          15 May 1999 - created
    *
    ***********************************************/

#include "cips.h"

#define FILL     200
#define FILL2    150
#define MOREROWS 100
#define DEGREESPERRADIAN 57.29577952

short **the_image;
short **out_image;

main(argc, argv)
   int argc;
   char *argv[];
{
   char in_name[MAX_NAME_LENGTH], 
        out_name[MAX_NAME_LENGTH];
   double tantheta;
   float scale;
   int  i, ii, j, jj, theta;
   int  space, value;
   long length1, length2, width1, width2;
   long bigxshift, xshift;
   short height, max;

      /* Check the command line and obtain 
         the parameters. */

   if(argc != 6){
    printf(
    "\n\nusage: iso in-file out-file theta space value"
    "\n    theta in integer degrees plus or minus"
    "\n    (stay away from 90 degrees)"
    "\n    space is spacing to draw lines on output"
    "\n    value = 0 use black dots on top of image"
    "\n    value = 1 use actual image values");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   strcpy(out_name, argv[2]);
   theta = atoi(argv[3]);
   space = atoi(argv[4]);
   value = atoi(argv[5]);

      /* check input file */

   if(does_not_exist(in_name)){
      printf("\nERROR input file %s does not exist",
             in_name);
      exit(0);
   }  /* ends if does_not_exist */

      /* get size of input file and calculate size 
         of output file.  output file will depend 
         on the angle theta */

   get_image_size(in_name, &length1, &width1);
   tantheta = tan( (double)(theta)/DEGREESPERRADIAN);
   bigxshift  = tantheta*(double)(length1);
   if(bigxshift < 0) bigxshift  = bigxshift * (-1);
   length2 = length1 + MOREROWS + MOREROWS;
   width2  = width1 + bigxshift;
   
   create_resized_image_file(in_name, out_name, 
                             length2, width2);

      /* allocate the image arrays */

   the_image = allocate_image_array(length1, width1);
   out_image = allocate_image_array(length2, width2);

      /* fill the output image with FILL */

   for(i=0; i<length2; i++)
      for(j=0; j<width2; j++)
         out_image[i][j] = FILL;

   read_image_array(in_name, the_image);


      /* Scale the height of the output image */

   max = 0;

   for(i=0; i<length1; i++){
      for(j=0; j<width1; j++){
         if(the_image[i][j] > max)
            max = the_image[i][j];
      }  /* ends loop over j */
   }  /* ends loop over i */


   if(max > MOREROWS)
      scale = (float)(max)/(float)(MOREROWS);
   else
      scale = max;

   if(scale < 1.0)
      scale = 1.0;

      /* Loop through the input image and place
         the pixels in the output image. */

   for(i=0; i<length1; i++){
      for(j=0; j<width1; j++){

         if(i%space == 0){
            ii      = i + MOREROWS;
            xshift  = tantheta*(double)(i);

            if(i == 0  &&  theta <= 0)
               jj = j + bigxshift + xshift;
            if(i == 0  &&  theta >  0)
               jj = j + xshift;
            if(i != 0  &&  theta <= 0)
               jj = j + bigxshift + xshift;
            if(i != 0  &&  theta >  0)
               jj = j + xshift;

            out_image[ii][jj] = the_image[i][j];

            height = the_image[i][j]/scale;
            lineup(out_image, ii, ii-height, jj);
            if(value == 0)
               out_image[ii-height][jj] = 0;
            else
               out_image[ii-height][jj] = 
                  the_image[i][j];
         }  /* ends if j%space  */

      }  /* ends loop over j */
   }  /* ends loop over i */


   write_image_array(out_name, out_image);
   free_image_array(the_image, length1);
   free_image_array(out_image, length2);

}  /* ends main  */



lineup(image, start_row, end_row, column)
   int   start_row, end_row, column;
   short **image;
{
   int i; 
   for(i=start_row; i>end_row; i--)
      image[i][column] = FILL2;
}  /* ends lineup */



