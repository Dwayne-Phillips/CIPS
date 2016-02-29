

       /***********************************************
       *
       *       file edge.c
       *
       *       Functions: This file contains
       *          detect_edges
       *          setup_masks
       *          get_edge_options
       *          perform_convolution
       *          quick_edge
       *
       *       Purpose:
       *          These functions implement several
       *          types of basic edge detection.
       *
       *       External Calls:
       *          utility.c - fix_edges
       *
       *       Modifications:
       *          27 January 1991 - created
       *          27 December 1992 - Fixed an error in
       *              how I did the 8 direction edge
       *              detectors.  I was only detecting
       *              edges in the last (the 7)
       *              direction.  I fixed this by
       *              setting the out_image to the sum
       *              only if the sum was greater than
       *              the out_image.  This is in the
       *              function perform_convolution.
       *          22 April 1998 - added capability to
       *              work an entire image at one time.
       *
       *************************************************/

#include "cips.h"

short quick_mask[3][3] =  {
       {-1,  0, -1},
       { 0,  4,  0},
       {-1,  0, -1} };


   /***************************
   *
   *   Directions for the masks
   *  3 2 1
   *  4 x 0
   *  5 6 7
   *
   ****************************/

   /* masks for kirsch operator */
short kirsch_mask_0[3][3] =  {
       { 5,  5,  5},
       {-3,  0, -3},
       {-3, -3, -3} };

short kirsch_mask_1[3][3] =  {
       {-3,  5,  5},
       {-3,  0,  5},
       {-3, -3, -3} };

short kirsch_mask_2[3][3] =  {
       {-3, -3,  5},
       {-3,  0,  5},
       {-3, -3,  5} };

short kirsch_mask_3[3][3] =  {
       {-3, -3, -3},
       {-3,  0,  5},
       {-3,  5,  5} };

short kirsch_mask_4[3][3] =  {
       {-3, -3, -3},
       {-3,  0, -3},
       { 5,  5,  5} };

short kirsch_mask_5[3][3] =  {
       {-3, -3, -3},
       { 5,  0, -3},
       { 5,  5, -3} };

short kirsch_mask_6[3][3] =  {
       { 5, -3, -3},
       { 5,  0, -3},
       { 5, -3, -3} };

short kirsch_mask_7[3][3] =  {
       { 5,  5, -3},
       { 5,  0, -3},
       {-3, -3, -3} };






   /* masks for prewitt operator */

short prewitt_mask_0[3][3] =  {
       { 1,  1,  1},
       { 1, -2,  1},
       {-1, -1, -1} };

short prewitt_mask_1[3][3] =  {
       { 1,  1,  1},
       { 1, -2, -1},
       { 1, -1, -1} };

short prewitt_mask_2[3][3] =  {
       { 1,  1, -1},
       { 1, -2, -1},
       { 1,  1, -1} };

short prewitt_mask_3[3][3] =  {
       { 1, -1, -1},
       { 1, -2, -1},
       { 1,  1,  1} };

short prewitt_mask_4[3][3] =  {
       {-1, -1, -1},
       { 1, -2,  1},
       { 1,  1,  1} };

short prewitt_mask_5[3][3] =  {
       {-1, -1,  1},
       {-1, -2,  1},
       { 1,  1,  1} };

short prewitt_mask_6[3][3] =  {
       {-1,  1,  1},
       {-1, -2,  1},
       {-1,  1,  1} };

short prewitt_mask_7[3][3] =  {
       { 1,  1,  1},
       {-1, -2,  1},
       {-1, -1,  1} };






   /* masks for sobel operator */

short sobel_mask_0[3][3] =  {
       { 1,  2,  1},
       { 0,  0,  0},
       {-1, -2, -1} };

short sobel_mask_1[3][3] =  {
       { 2,  1,  0},
       { 1,  0, -1},
       { 0, -1, -2} };

short sobel_mask_2[3][3] =  {
       { 1,  0, -1},
       { 2,  0, -2},
       { 1,  0, -1} };
short sobel_mask_3[3][3] =  {
       { 0, -1, -2},
       { 1,  0, -1},
       { 2,  1,  0} };

short sobel_mask_4[3][3] =  {
       {-1, -2, -1},
       { 0,  0,  0},
       { 1,  2,  1} };

short sobel_mask_5[3][3] =  {
       {-2, -1,  0},
       {-1,  0,  1},
       { 0,  1,  2} };

short sobel_mask_6[3][3] =  {
       {-1,  0,  1},
       {-2,  0,  2},
       {-1,  0,  1} };

short sobel_mask_7[3][3] =  {
       { 0,  1,  2},
       {-1,  0,  1},
       {-2, -1,  0} };






  /**************************************************
  *
  *   detect_edges(...
  *
  *   This function detects edges in an area of one
  *   image and sends the result to another image
  *   on disk.  It reads the input image from disk,
  *   calls a convolution function, and then writes
  *   the result out to disk.  If needed, it
  *   allocates space on disk for the output image.
  *
  ***************************************************/




detect_edges(the_image, out_image,
             detect_type, threshold, high,
             rows, cols, bits_per_pixel)
   int    detect_type, high, threshold;
   long   rows, cols, bits_per_pixel;
   short  **the_image, **out_image;

{
   perform_convolution(the_image, out_image,
                       detect_type, threshold,
                       rows, cols, 
                       bits_per_pixel, 
                       high);
   fix_edges(out_image, 1, rows, cols);
}  /* ends detect_edges */







     /**********************************************************
     *
     *   perform_convolution(...
     *
     *   This function performs convolution between the input
     *   image and 8 3x3 masks.  The result is placed in
     *   the out_image.
     *
     ********************************************************/

perform_convolution(image, out_image,
                    detect_type, threshold,
                    rows, cols, bits_per_pixel, high)
   short **image,
         **out_image;
   int   detect_type, high, threshold;
   long  rows, cols, bits_per_pixel;
{

   char response[80];
   int a,
       b,
       i,
       is_present,
       j,
       sum; 

   short  mask_0[3][3],
          mask_1[3][3],
          mask_2[3][3],
          mask_3[3][3],
          mask_4[3][3],
          mask_5[3][3],
          mask_6[3][3],
          mask_7[3][3],
          max,
          min,
          new_hi,
          new_low;


   setup_masks(detect_type, mask_0, mask_1,
               mask_2, mask_3, mask_4, mask_5,
               mask_6, mask_7);

   new_hi  = 250;
   new_low = 16;
   if(bits_per_pixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

   min = 0;
   max = 255;
   if(bits_per_pixel == 4)
      max = 16;

     /* clear output image array */
   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = 0;


   printf("\n ");

   for(i=1; i<rows-1; i++){
if( (i%10) == 0){ printf("%4d", i); }
      for(j=1; j<cols-1; j++){


         /* Convolve for all 8 directions */

         /* 0 direction */

      sum = 0;
      for(a=-1; a<2; a++){
         for(b=-1; b<2; b++){
            sum = sum + image[i+a][j+b] *
                  mask_0[a+1][b+1];
         }
      }
         if(sum > max) sum = max;
         if(sum < 0)   sum = 0;
            /* Correction 12-27-92
               see file header for
               details. */
      if(sum > out_image[i][j])
         out_image[i][j]   = sum;


         /* 1 direction */

      sum = 0;
      for(a=-1; a<2; a++){
         for(b=-1; b<2; b++){
            sum = sum + image[i+a][j+b] * mask_1[a+1][b+1];
         }
      }
         if(sum > max) sum = max;
         if(sum < 0)   sum = 0;
            /* Correction 12-27-92
               see file header for
               details. */
      if(sum > out_image[i][j])
         out_image[i][j]   = sum;


         /* 2 direction */

      sum = 0;
      for(a=-1; a<2; a++){
         for(b=-1; b<2; b++){
            sum = sum + image[i+a][j+b] * mask_2[a+1][b+1];
         }
      }
         if(sum > max) sum = max;
         if(sum < 0)   sum = 0;
            /* Correction 12-27-92
               see file header for
               details. */
      if(sum > out_image[i][j])
         out_image[i][j]   = sum;


         /* 3 direction */

      sum = 0;
      for(a=-1; a<2; a++){
         for(b=-1; b<2; b++){
            sum = sum + image[i+a][j+b] * mask_3[a+1][b+1];
         }
      }
         if(sum > max) sum = max;
         if(sum < 0)   sum = 0;
            /* Correction 12-27-92
               see file header for
               details. */
      if(sum > out_image[i][j])
         out_image[i][j]   = sum;


         /* 4 direction */

      sum = 0;
      for(a=-1; a<2; a++){
         for(b=-1; b<2; b++){
            sum = sum + image[i+a][j+b] * mask_4[a+1][b+1];
         }
      }
         if(sum > max) sum = max;
         if(sum < 0)   sum = 0;
            /* Correction 12-27-92
               see file header for
               details. */
      if(sum > out_image[i][j])
         out_image[i][j]   = sum;


         /* 5 direction */

      sum = 0;
      for(a=-1; a<2; a++){
         for(b=-1; b<2; b++){
            sum = sum + image[i+a][j+b] * mask_5[a+1][b+1];
         }
      }
         if(sum > max) sum = max;
         if(sum < 0)   sum = 0;
            /* Correction 12-27-92
               see file header for
               details. */
      if(sum > out_image[i][j])
         out_image[i][j]   = sum;


         /* 6 direction */
      sum = 0;
      for(a=-1; a<2; a++){
         for(b=-1; b<2; b++){
            sum = sum + image[i+a][j+b] * mask_6[a+1][b+1];
         }
      }
         if(sum > max) sum = max;
         if(sum < 0)   sum = 0;
            /* Correction 12-27-92
               see file header for
               details. */
      if(sum > out_image[i][j])
         out_image[i][j]   = sum;


         /* 7 direction */

      sum = 0;
      for(a=-1; a<2; a++){
         for(b=-1; b<2; b++){
            sum = sum + image[i+a][j+b] * mask_7[a+1][b+1];
         }
      }
         if(sum > max) sum = max;
         if(sum < 0)   sum = 0;
            /* Correction 12-27-92
               see file header for
               details. */
      if(sum > out_image[i][j])
         out_image[i][j]   = sum;


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

}  /* ends perform_convolution */







     /*******************************************
     *
     *   quick_edge(...
     *
     *   This function finds edges by using
     *   a single 3x3 mask.
     *
     *******************************************/


quick_edge(the_image, out_image,
           threshold, high, rows, cols, bits_per_pixel)
   int    high, threshold;
   long   rows, cols, bits_per_pixel;
   short  **the_image, **out_image;

{
   short  a, b, i, j, k,
          length, max, new_hi, new_low,
          sum, width;


   new_hi  = 250;
   new_low = 16;
   if(bits_per_pixel == 4){
       new_hi  = 10;
       new_low = 3;
   }

   max = 255;
   if(bits_per_pixel == 4)
      max = 16;

         /* Do convolution over image array */
   printf("\n");
   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%d ", i);
      for(j=1; j<cols-1; j++){
         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum +
                     the_image[i+a][j+b] *
                     quick_mask[a+1][b+1];
            }
         }
         if(sum < 0)   sum = 0;
         if(sum > max) sum = max;
         out_image[i][j]   = sum;

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

   fix_edges(out_image, 1, 
             rows-1, cols-1); 

}  /* ends quick_edge */








   /***********************************************
    *
    *    setup_masks(...
    *
    *    This function copies the mask values defined
    *    at the top of this file into the mask
    *    arrays mask_0 through mask_7.
    *
    ***********************************************/



setup_masks(detect_type, mask_0, mask_1, mask_2, mask_3,
            mask_4, mask_5, mask_6, mask_7)
   int    detect_type;
   short  mask_0[3][3],
          mask_1[3][3],
          mask_2[3][3],
          mask_3[3][3],
          mask_4[3][3],
          mask_5[3][3],
          mask_6[3][3],
          mask_7[3][3];
{
   int i, j;

   if(detect_type == KIRSCH){
      for(i=0; i<3; i++){
        for(j=0; j<3; j++){
          mask_0[i][j] = kirsch_mask_0[i][j];
          mask_1[i][j] = kirsch_mask_1[i][j];
          mask_2[i][j] = kirsch_mask_2[i][j];
          mask_3[i][j] = kirsch_mask_3[i][j];
          mask_4[i][j] = kirsch_mask_4[i][j];
          mask_5[i][j] = kirsch_mask_5[i][j];
          mask_6[i][j] = kirsch_mask_6[i][j];
          mask_7[i][j] = kirsch_mask_7[i][j];
        }
      }
   }  /* ends if detect_type == KIRSCH */


   if(detect_type == PREWITT){
      for(i=0; i<3; i++){
        for(j=0; j<3; j++){
          mask_0[i][j] = prewitt_mask_0[i][j];
          mask_1[i][j] = prewitt_mask_1[i][j];
          mask_2[i][j] = prewitt_mask_2[i][j];
          mask_3[i][j] = prewitt_mask_3[i][j];
          mask_4[i][j] = prewitt_mask_4[i][j];
          mask_5[i][j] = prewitt_mask_5[i][j];
          mask_6[i][j] = prewitt_mask_6[i][j];
          mask_7[i][j] = prewitt_mask_7[i][j];
        }
      }
   }  /* ends if detect_type == PREWITT */


   if(detect_type == SOBEL){
      for(i=0; i<3; i++){
        for(j=0; j<3; j++){
          mask_0[i][j] = sobel_mask_0[i][j];
          mask_1[i][j] = sobel_mask_1[i][j];
          mask_2[i][j] = sobel_mask_2[i][j];
          mask_3[i][j] = sobel_mask_3[i][j];
          mask_4[i][j] = sobel_mask_4[i][j];
          mask_5[i][j] = sobel_mask_5[i][j];
          mask_6[i][j] = sobel_mask_6[i][j];
          mask_7[i][j] = sobel_mask_7[i][j];
        }
      }
   }  /* ends if detect_type == SOBEL */

}  /* ends setup_masks */
