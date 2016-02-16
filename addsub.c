
   /***********************************************
   *
   *       file addsub.c
   *
   *       Functions: This file contains
   *          add_image_array
   *          subtract_image_array
   *
   *       Purpose:
   *          These functions implement
   *          image addition and subtraction.
   *
   *       External Calls:
   *          none
   *
   *       Modifications:
   *          1 April 1992 - created
   *
   *************************************************/

#include "cips.h"

     /*******************************************
     *
     *   add_image_array(...
     *
     *   This function adds two image arrays.
     *   The image array out_image will hold
     *   the result.
     *
     *******************************************/

add_image_array(the_image, out_image, rows, cols, max)
   int    rows, cols;
   short  **the_image,
          **out_image,
          max;
{
   int    i, j;

   for(i=0; i<rows; i++){
      for(j=0; j<cols; j++){
         out_image[i][j] = the_image[i][j] +
                           out_image[i][j];
         if(out_image[i][j] > max)
            out_image[i][j] = max;
      }  /* ends loop over j */
   }  /* ends loop over i */



}  /* ends add_image_array */





     /*******************************************
     *
     *   subtract_image_array(...
     *
     *   This function subtracts two image arrays.
     *   The image array out_image will hold
     *   the result.
     *
     *******************************************/

subtract_image_array(the_image, out_image, rows, cols)
   int    rows, cols;
   short  **the_image,
          **out_image;
{
   int    i, j, length, width;

   for(i=0; i<rows; i++){
      for(j=0; j<cols; j++){
         out_image[i][j] = the_image[i][j] -
                           out_image[i][j];
         if(out_image[i][j] < 0)
            out_image[i][j] = 0;
      }  /* ends loop over j */
   }  /* ends loop over i */

}  /* ends subtract_image_array */
