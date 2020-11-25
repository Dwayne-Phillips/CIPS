
    /***********************************************
    *
    *    file edge2.c
    *
    *    Functions: This file contains
    *       homogeneity
    *       difference_edge
    *       contrast_edge
    *       range
    *       variance
    *
    *    Purpose:
    *       These functions implement several
    *       types of advanced edge detection.
    *
    *    External Calls:
    *       utility.c - sort_elements
    *
    *    Modifications:
    *       26 March 1991 - created
    *       30 December 1992 - added the range and
    *           variance edge detectors.
    *       May 10, 1998 - modified routines to work
    *           with an entire image in one array.
    *
    *************************************************/

#include "cips.h"

int sort_elements();

short e_mask[3][3] = {
       {-9,  0, -9},
       { 0, 36,  0},
       {-9,  0, -9} };

short contrast[3][3] = {
   {  1,  1,  1},
   {  1,  1,  1},
   {  1,  1,  1}};


   /**************************************************
   *
   *   homogeneity(...
   *
   *   This function performs edge detection by looking
   *   for the absence of an edge.  The center of a
   *   3x3 area is replaced by the absolute value of
   *   the max difference between the center point
   *   and its 8 neighbors.
   *
   ***************************************************/


int homogeneity(the_image, out_image,
            rows, cols, bits_per_pixel,
            threshold, high)
   int    high, threshold;
   short  **the_image, **out_image;
   long   rows, cols, bits_per_pixel;
{
   int a, b, absdiff, absmax, diff, i, j,
       length, max, max_diff, new_hi, new_low, width;


   new_hi  = 250;
   new_low = 16;
   if(bits_per_pixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

   max = 255;
   if(bits_per_pixel == 4)
      max = 16;

   for(i=0; i<rows; i++){
      for(j=0; j<cols; j++){
         out_image[i][j] = 0;
      }
   }

   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%4d", i);
      for(j=1; j<cols-1; j++){

          max_diff = 0;
          for(a=-1; a<=1; a++){
             for(b=-1; b<=1; b++){

                diff = the_image[i][j] -
                        the_image[i+a][j+b];
                absdiff = abs(diff);
                if(absdiff > max_diff)
                   max_diff = absdiff;

             }  /* ends loop over b */
          }  /* ends loop over a */

          out_image[i][j] = max_diff;
      }  /* ends loop over j */
   }  /* ends loop over i */


     /* if desired, threshold the output image */
   if(threshold == 1){
       for(i=0; i<rows; i++){
          for(j=0; j<cols; j++){
             if(out_image[i][j] > high){
                  out_image[i][j] = new_hi;
             }
             else{
                  out_image[i][j] = new_low;
             }
          }
       }
   }  /* ends if threshold == 1 */

return(1);
} /* ends homogeneity */



   /**************************************************
   *
   *   difference_edge(...
   *
   *   This function performs edge detection by looking
   *   at the differences in the pixels that surround
   *   the center point of a 3x3 area.  It replaces the
   *   center point with the absolute value of the
   *   max difference of:
   *      upper left - lower right
   *      upper right - lower left
   *      left - right
   *      top - bottom
   *
   ***************************************************/

int difference_edge(the_image, out_image,
                rows, cols, bits_per_pixel,
                threshold, high)
   int    high, threshold;
   short  **the_image, **out_image;
   long   rows, cols, bits_per_pixel;
{
   int a, b, absdiff, absmax, diff, i, j,
       length, max, max_diff, new_hi, new_low, width;

   new_hi  = 250;
   new_low = 16;
   if(bits_per_pixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

   max = 255;
   if(bits_per_pixel == 4)
      max = 16;

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = 0;

   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%4d", i);
      for(j=1; j<cols-1; j++){

          max_diff = 0;
          absdiff = abs(the_image[i-1][j-1] -
                         the_image[i+1][j+1]);
          if(absdiff > max_diff) max_diff = absdiff;

          absdiff = abs(the_image[i-1][j+1] -
                         the_image[i+1][j-1]);
          if(absdiff > max_diff) max_diff = absdiff;

          absdiff = abs(the_image[i][j-1] -
                         the_image[i][j+1]);
          if(absdiff > max_diff) max_diff = absdiff;

          absdiff = abs(the_image[i-1][j] -
                         the_image[i+1][j]);
          if(absdiff > max_diff) max_diff = absdiff;


          out_image[i][j] = max_diff;

      }  /* ends loop over j */
   }  /* ends loop over i */



     /* if desired, threshold the output image */
   if(threshold == 1){
       for(i=0; i<rows; i++){
          for(j=0; j<cols; j++){
             if(out_image[i][j] > high){
                  out_image[i][j] = new_hi;
             }
             else{
                  out_image[i][j] = new_low;
             }
          }
       }
   }  /* ends if threshold == 1 */

return(1);
} /* ends difference_edge */





   /**************************************************
   *
   *   contrast_edge(...
   *
   *   The edge detector uses the basic quick edge
   *   detector mask and then divides the result
   *   by a contrast smooth mask.  This implements
   *   Johnson's contrast based edge detector.
   *
   ***************************************************/

int contrast_edge(the_image, out_image,
              rows, cols, bits_per_pixel,
              threshold, high)
   int    high, threshold;
   short  **the_image, **out_image;
   long   rows, cols, bits_per_pixel;
{
   int ad, d;
   int a, b, absdiff, absmax, diff, i, j,
       length, max, new_hi, new_low,
       sum_d, sum_n, width;

   new_hi  = 250;
   new_low = 16;
   if(bits_per_pixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

   max = 255;
   if(bits_per_pixel == 4)
      max = 16;

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = 0;

   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%4d", i);
      for(j=1; j<cols-1; j++){

         sum_n = 0;
         sum_d = 0;

         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum_n = sum_n + the_image[i+a][j+b] *
                       e_mask[a+1][b+1];
               sum_d = sum_d + the_image[i+a][j+b] *
                       contrast[a+1][b+1];
            }
         }

         d = sum_d / 9;
         if(d == 0)
            d = 1;

         out_image[i][j] = sum_n/d;

         if(out_image[i][j] > max)
            out_image[i][j] = max;
         if(out_image[i][j] < 0)
            out_image[i][j] = 0;


      }  /* ends loop over j */
   }  /* ends loop over i */



     /* if desired, threshold the output image */
   if(threshold == 1){
       for(i=0; i<rows; i++){
          for(j=0; j<cols; j++){
             if(out_image[i][j] > high){
                  out_image[i][j] = new_hi;
             }
             else{
                  out_image[i][j] = new_low;
             }
          }
       }
   }  /* ends if threshold == 1 */

return(1);
} /* ends contrast_edge */





     /*******************************************
     *
     *   range(..
     *
     *   This edge detector performs the
     *   range operation.
     *   It replaces the pixel at the center of a
     *   3x3, 5x5, etc. area with the max - min
     *   for that area.
     *
     *******************************************/

int range(the_image, out_image,
      rows, cols, bits_per_pixel,
      size, threshold, high)
   int    high, threshold, size;
   short  **the_image,
          **out_image;
   long   rows, cols, bits_per_pixel;
{
   int    a, b, count, i, j, k,
          new_hi, new_low, length,
          sd2, sd2p1, ss, width;
   short  *elements;

   sd2   = size/2;
   sd2p1 = sd2 + 1;

      /**********************************************
      *
      *   Allocate the elements array large enough
      *   to hold size*size shorts.
      *
      **********************************************/

   ss       = size*size;
   elements = (short *) malloc(ss * sizeof(short));

   new_hi  = 250;
   new_low = 16;
   if(bits_per_pixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

      /***************************
      *
      *   Loop over image array
      *
      ****************************/

   printf("\n");
   for(i=sd2; i<rows-sd2; i++){
      if( (i%10) == 0) printf("%4d ", i);
      for(j=sd2; j<cols-sd2; j++){
         count = 0;
         for(a=-sd2; a<sd2p1; a++){
            for(b=-sd2; b<sd2p1; b++){
               elements[count] = the_image[i+a][j+b];
               count++;
            }
         }
         sort_elements(elements, &ss);
         out_image[i][j] = elements[ss-1]-elements[0];
      }  /* ends loop over j */
   }  /* ends loop over i */

     /* if desired, threshold the output image */
   if(threshold == 1){
       for(i=0; i<rows; i++){
          for(j=0; j<cols; j++){
             if(out_image[i][j] > high){
                  out_image[i][j] = new_hi;
             }
             else{
                  out_image[i][j] = new_low;
             }
          }
       }
   }  /* ends if threshold == 1 */

   free(elements);

return(1);
}  /* ends range */





   /**************************************************
   *
   *   variance(...
   *
   *   This function replaces the pixel in the center
   *   of a 3x3 area with the square root of the sum
   *   of squares of the differences between the
   *   center pixel and its eight neighbors.
   *
   ***************************************************/

int variance(the_image, out_image,
         rows, cols, bits_per_pixel,
         threshold, high)
   int    high, threshold;
   short  **the_image,
          **out_image;
   long   rows, cols, bits_per_pixel;
{
   int      a, b, i, j, length,
            max, new_hi, new_low, width;
   long     diff;
   unsigned long sum, tmp;

   new_hi  = 250;
   new_low = 16;
   if(bits_per_pixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

   max = 255;
   if(bits_per_pixel == 4)
      max = 16;

   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%4d", i);
      for(j=1; j<cols-1; j++){
         sum = 0;
         for(a=-1; a<=1; a++){
             for(b=-1; b<=1; b++){
                if( a!=0  &&  b!=0){
                 diff = 0;
                 diff = the_image[i][j] -
                        the_image[i+a][j+b];
                 tmp = diff*diff;
                 sum = sum + tmp;
                }
             }
         }
         if(sum < 0)
            printf("\nWHAT? sum < 0, %ld ,diff=%d ", sum, diff);
         sum = sqrt(sum);
         if(sum > max) sum = max;
         out_image[i][j] = sum;
      }  /* ends loop over j */
   }  /* ends loop over i */

      /* if desired, threshold the output image */
   if(threshold == 1){
       for(i=0; i<rows; i++){
          for(j=0; j<cols; j++){
             if(out_image[i][j] > high){
                  out_image[i][j] = new_hi;
             }
             else{
                  out_image[i][j] = new_low;
             }
          }
       }
   }  /* ends if threshold == 1 */

return(1);
} /* ends variance */
