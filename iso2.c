

    /*********************************************
    *
    *       file iso2.c
    *
    *       Functions: This file contains
    *          main
    *          lineup
    *
    *       Purpose:
    *          This program performs an isometric
    *          transform on a 3D image.
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
    *          24 May 1999 - created
    *
    ***********************************************/

#include "cips.h"

#define FILL     200
#define FILL2    150
#define MOREROWS 100
#define DEGREESPERRADIAN 57.29577952

int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();
int create_resized_image_file();
int lineup();


short **the_image;
short **out_image;

int main(argc, argv)
   int argc;
   char *argv[];
{
   char in_name[MAX_NAME_LENGTH],
        out_name[MAX_NAME_LENGTH];
   double tantheta, cosa, cosb, sina, sinb;
   float scale;
   int  alpha, beta, col0,
        i, ii, j, jj,
        row0, space, value;
   long length1, length2, width1, width2;
   short height, max;

      /* Check the command line and obtain
         the parameters. */

   if(argc != 7){
    printf(
     "\n\nusage: iso2 in-file out-file "
           "alpha beta space value"
     "\n    alpha is angle down from horizontal"
     "\n    beta is angle up from horizontal"
     "\n    space is spacing to draw lines on output"
     "\n    value = 0 use black dots on top of image"
     "\n    value = 1 use actual image values\n");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   strcpy(out_name, argv[2]);
   alpha = atoi(argv[3]);
   beta  = atoi(argv[4]);
   space = atoi(argv[5]);
   value = atoi(argv[6]);

      /* check input file */

   if(does_not_exist(in_name)){
      printf("\nERROR input file %s does not exist",
             in_name);
      exit(0);
   }  /* ends if does_not_exist */

      /* calculate the sin and cosine of the angles
         one time */

   sina = sin((double)(alpha)/DEGREESPERRADIAN);
   cosa = cos((double)(alpha)/DEGREESPERRADIAN);
   sinb = sin((double)(beta)/DEGREESPERRADIAN);
   cosb = cos((double)(beta)/DEGREESPERRADIAN);

      /* get size of input file and calculate size
         of output file.  output file will depend
         on the angle theta */

   get_image_size(in_name, &length1, &width1);
   length2 = (double)(width1)*sinb +
             (double)(length1)*sina +
             MOREROWS + MOREROWS;
   width2  = (double)(width1)*cosb +
             (double)(length1)*cosa;

   row0 = MOREROWS/2 +
          MOREROWS +
          (double)(width1)*sinb;
   col0 = 0;

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
            ii = row0 +
                 (double)(i)*sina -
                 (double)(j)*sinb;
            jj = col0 +
                 (double)(i)*cosa +
                 (double)(j)*cosb;

            out_image[ii][jj] = the_image[i][j];

            height = the_image[i][j]/scale;
            lineup(out_image, ii, ii-height, jj);
            if(value == 0)
               out_image[ii-height][jj] = 0;
            else
               out_image[ii-height][jj] =
                  the_image[i][j];
         }  /* ends if j mod space */

      }  /* ends loop over j */
   }  /* ends loop over i */

   write_image_array(out_name, out_image);
   free_image_array(the_image, length1);
   free_image_array(out_image, length2);

}  /* ends main  */



int lineup(image, start_row, end_row, column)
   int   start_row, end_row, column;
   short **image;
{
   int i;
   for(i=start_row; i>end_row; i--)
      image[i][column] = FILL2;
return(1);
}  /* ends lineup */
