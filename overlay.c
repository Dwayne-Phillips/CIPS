

    /*********************************************
    *
    *       file overlay.c
    *
    *       Functions: This file contains
    *          non_zero_overlay
    *          zero_overlay
    *          greater_overlay
    *          less_overlay
    *          average_overlay
    *
    *       Purpose:
    *          These functions implement the 
    *          functions that overlay one image 
    *          on top of another image.
    *
    *       External Calls:
    *          none
    *
    *       Modifications:
    *          6 March 1993 - created
    *         22 August 1998 - modified to work on 
    *               entire images at once.
    *
    *********************************************/


#include "cips.h"



   /**********************************************
   *
   *   non_zero_overlay(...
   *
   *   This function overlays in1 on top of in2
   *   and writes the result to the output image.
   *   It writes any non-zero pixel from in1 on top
   *   of in2.
   *
   ***********************************************/

non_zero_overlay(the_image, out_image,
                 rows, cols)
   short  **the_image,
          **out_image;
   long   cols, rows;
{
   int    i, j;

   for(i=0; i<rows; i++){
      if ( (i%10) == 0) printf(" %d", i);
      for(j=0; j<cols; j++){
         if(the_image[i][j] != 0)
            out_image[i][j] = the_image[i][j];
      }  /* ends loop over j */
   }  /* ends loop over i */

} /* ends non_zero_overlay */





   /**********************************************
   *
   *   zero_overlay(...
   *
   *   This function overlays in1 on top of in2
   *   and writes the result to the output image.
   *   It writes any zero pixel from in1 on top
   *   of in2.
   *
   ***********************************************/

zero_overlay(the_image, out_image,
             rows, cols)
   short **the_image,
         **out_image;
   long  cols, rows;
{
   int    i, j;

   for(i=0; i<rows; i++){
      if ( (i%10) == 0) printf(" %d", i);
      for(j=0; j<cols; j++){
         if(the_image[i][j] == 0)
            out_image[i][j] = the_image[i][j];
      }  /* ends loop over j */
   }  /* ends loop over i */

} /* ends zero_overlay */





   /**********************************************
   *
   *   greater_overlay(...
   *
   *   This function overlays in1 on top of in2
   *   and writes the result to the output image.
   *   It writes in1 on top of in2 if the value of
   *   in1 is greater than in2.
   *
   ***********************************************/

greater_overlay(the_image, out_image,
                rows, cols)
   short **the_image,
         **out_image;
   long  cols, rows;
{
   int    i, j;

   for(i=0; i<rows; i++){
      if ( (i%10) == 0) printf(" %d", i);
      for(j=0; j<cols; j++){
         if(the_image[i][j] > out_image[i][j])
            out_image[i][j] = the_image[i][j];
      }  /* ends loop over j */
   }  /* ends loop over i */

} /* ends greater_overlay */





   /**********************************************
   *
   *   less_overlay(...
   *
   *   This function overlays in1 on top of in2
   *   and writes the result to the output image.
   *   It writes in1 on top of in2 if the value of
   *   in1 is less than in2.
   *
   ***********************************************/

less_overlay(the_image, out_image,
             rows, cols)
   short **the_image,
         **out_image;
   long  cols, rows;
{
   int    i, j;


   for(i=0; i<rows; i++){
      if ( (i%10) == 0) printf(" %d", i);
      for(j=0; j<cols; j++){
         if(the_image[i][j] < out_image[i][j])
            out_image[i][j] = the_image[i][j];
      }  /* ends loop over j */
   }  /* ends loop over i */

} /* ends less_overlay */





   /**********************************************
   *
   *   average_overlay(...
   *
   *   This function mixes in1 and in2
   *   and writes the result to the output image.
   *   It writes the average of in1 and in2 to the
   *   output image.
   *
   ***********************************************/

average_overlay(the_image, out_image,
                rows, cols)
   short **the_image,
         **out_image;
   long  cols, rows;
{
   int    i, j;

   for(i=0; i<rows; i++){
      if ( (i%10) == 0) printf(" %d", i);
      for(j=0; j<cols; j++){
         out_image[i][j] =
            (the_image[i][j] + out_image[i][j])/2;
      }  /* ends loop over j */
   }  /* ends loop over i */

} /* ends average_overlay */
