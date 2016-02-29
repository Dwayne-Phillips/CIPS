

    /***********************************************
    *
    *    file d:\cips\edge3.c
    *
    *    Functions: This file contains
    *       gaussian_edge
    *       enhance_edges
    *
    *    Purpose:
    *       These functions implement several
    *       types of advanced edge detection.
    *
    *    External Calls:
    *       none
    *
    *    Modifications:
    *       26 March 1991 - created
    *       May 10, 1998 - modified routines to work
    *           with an entire image in one array.
    *
    ***********************************************/






#include "cips.h"


short enhance_mask[3][3] =  {
       {-1,  0, -1},
       { 0,  4,  0},
       {-1,  0, -1} };



short g7[7][7] = {
     {  0,  0, -1, -1, -1,  0,  0},
     {  0, -2, -3, -3, -3, -2,  0},
     { -1, -3,  5,  5,  5, -3, -1},
     { -1, -3,  5, 16,  5, -3, -1},
     { -1, -3,  5,  5,  5, -3, -1},
     {  0, -2, -3, -3, -3, -2,  0},
     {  0,  0, -1, -1, -1,  0,  0}};

short g9[9][9] = {
   {  0,  0,  0,  -1, -1, -1,  0,  0,  0},
   {  0, -2, -3,  -3, -3, -3, -3, -2,  0},
   {  0, -3, -2,  -1, -1, -1, -2, -3,  0},
   { -1, -3, -1,   9,  9,  9, -1, -3, -1},
   { -1, -3, -1,   9, 19,  9, -1, -3, -1},
   { -1, -3, -1,   9,  9,  9, -1, -3, -1},
   {  0, -3, -2,  -1, -1, -1, -2, -3,  0},
   {  0, -2, -3,  -3, -3, -3, -3, -2,  0},
   {  0,  0,  0,  -1, -1, -1,  0,  0,  0}};



   /************************************************
   *
   *   gaussian_edge(...
   *
   *
   *************************************************/

gaussian_edge(the_image, out_image,
              rows, cols, bits_per_pixel,
              size, threshold, high)
   int    high, size, threshold;
   short  **the_image, 
          **out_image;
   long   rows, cols, bits_per_pixel;
{
   char response[80];
   long sum;
   int  a, b, absdiff, absmax, diff, i, j,
        length, lower, max, new_hi, new_low,
        scale, starti, stopi, startj, stopj, 
        upper, width;

   new_hi  = 250;
   new_low = 16;
   if(bits_per_pixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

   max = 255;
   if(bits_per_pixel == 4)
      max = 16;


   if(size == 7){
      lower = -3;
      upper =  4;
      starti =  3;
      startj =  3;
      stopi  =  rows-3;
      stopj  =  cols-3;
      scale =  2;
   }

   if(size == 9){
      lower = -4;
      upper =  5;
      starti =  4;
      startj =  4;
      stopi  =  rows-4;
      stopj  =  cols-4;
      scale =  2;
   }


   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = 0;


   for(i=starti; i<stopi; i++){
      if ( (i%10) == 0) printf(" i=%d", i);
      for(j=startj; j<stopj; j++){

      sum = 0;

      for(a=lower; a<upper; a++){
         for(b=lower; b<upper; b++){
            if(size == 7)
               sum = sum + the_image[i+a][j+b] *
                     g7[a+3][b+3];
            if(size == 9)
               sum = sum + the_image[i+a][j+b] *
                     g9[a+4][b+4];
         } /* ends loop over a */
      }  /* ends loop over b */

      if(sum < 0) sum = 0;
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

} /* ends gaussian_edge */






     /*******************************************
     *
     * enhance_edges(...
     *
     * This function enhances the edges in an
     * input image and writes the enhanced
     * result to an output image.  It operates
     * much the same way as detect_edges
     * except it uses only one type of mask.
     *
     * The threshold and high parameters perform
     * a different role in this function.  The
     * threshold parameter does not exist.  The
     * high parameter determines if the edge is
     * strong enough to enhance or change the
     * input image.
     *
     *******************************************/


enhance_edges(the_image, out_image,
              rows, cols, bits_per_pixel, high)
   int    high;
   short  **the_image, 
          **out_image;
   long   rows, cols, bits_per_pixel;

{
   int    a, b, i, j, k,
          length, max, new_hi,
          new_lo, sum, width;

   max = 255;
   if(bits_per_pixel == 4)
      max = 16;


         /* Do convolution over image array */
   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%d ", i);
      for(j=1; j<cols-1; j++){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum +
                     the_image[i+a][j+b] *
                     enhance_mask[a+1][b+1];
            }
         }
         if(sum < 0)   sum = 0;
         if(sum > max) sum = max;
         if(sum > high)
            out_image[i][j] = max;
         else
            out_image[i][j] = the_image[i][j];
      }  /* ends loop over j */
   }  /* ends loop over i */

}  /* ends enhance_edges */
