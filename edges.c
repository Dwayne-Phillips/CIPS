
   /*************************** 
   * 
   *   edges.c 
   *   COMPOSITE FILE COMPRISING: 
   *   edge.c 
   *   edge2.c 
   *   edge3.c 
   * 
   ***************************\ 




       /***********************************************
       *
       *       file d:\cips\edge.c
       *
       *       Functions: This file contains
       *          detect_edges
       *          setup_masks
       *          get_edge_options
       *          perform_convolution
       *          quick_edge
       *          fix_edges
       *
       *       Purpose:
       *          These functions implement several
       *          types of basic edge detection.
       *
       *       External Calls:
       *          wtiff.c - round_off_image_size
       *                    create_file_if_needed
       *                    write_array_into_tiff_image
       *          tiff.c - read_tiff_header
       *          rtiff.c - read_tiff_image
       *          numcvrt.c - get_integer
       *
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



#ifdef NEVER


   /***********************************************
    *
    *    get_edge_options(...
    *
    *    This function queries the user for the
    *    parameters need to perform edge
    *    detection.
    *
    ***********************************************/


get_edge_options(detect_type, threshold, high, size)
    int *detect_type, *high, *size, *threshold;
{
    int not_finished, response;
    not_finished = 1;
    while(not_finished){

      printf("\nThe Edge Detector options are:\n");
      printf("\n\t1.  Type of edge detector is %d", *detect_type);
      printf("\n\t      (recall 1=Prewitt     2=Kirsch");
      printf("\n\t              3=Sobel       4=quick");
      printf("\n\t              5=homogeneity 6=difference");
      printf("\n\t              7=contrast    8=gaussian");
      printf("\n\t              10=range      11=variance");
      printf("\n\t2.  Threshold output is %d (0=off 1=on)", *threshold);
      printf("\n\t3.  High threshold is %d", *high);
      printf("\n\t4.  Size is %d (gaussian only)", *size);
      printf("\n\nEnter choice (0 = no change) _\b");


      get_integer(&response);

      if(response == 0){
        not_finished = 0;
      }


      if(response == 1){
        printf("\n\nEnter type of edge detector");
        printf("\n\t      (recall 1=Prewitt     2=Kirsch");
        printf("\n\t              3=Sobel       4=quick");
        printf("\n\t              5=homogeneity 6=difference");
        printf("\n\t              7=contrast    8=gaussian");
        printf("\n\t              10=range      11=variance");
        printf("\n  _\b");
        get_integer(detect_type);
      }

      if(response == 2){
        printf("\n\nEnter threshold output (0=off 1=on)");
        printf("\n  _\b");
        get_integer(threshold);
      }

      if(response == 3){
        printf("\n\nEnter high threshold");
        printf("\n  _\b");
        get_integer(high);
      }

      if(response == 4){
        printf("\n\nEnter size for gaussian (7 or 9)");
        printf("\n  _\b");
        get_integer(size);
      }
    }  /* ends while not_finished */

}  /* ends get_edge_options */

#endif





    /***********************************************
    *
    *    file d:\cips\edge2.c
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
    *       wtiff.c - round_off_image_size
    *                 create_file_if_needed
    *                 write_array_into_tiff_image
    *       tiff.c - read_tiff_header
    *       rtiff.c - read_tiff_image
    *       numcvrt.c - get_integer
    *       edge.c - fix_edges
    *
    *    Modifications:
    *       26 March 1991 - created
    *       30 December 1992 - added the range and
    *           variance edge detectors.
    *       May 10, 1998 - modified routines to work
    *           with an entire image in one array.
    *
    *************************************************/



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


homogeneity(the_image, out_image,
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

difference_edge(the_image, out_image,
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

contrast_edge(the_image, out_image,
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

range(the_image, out_image,
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

variance(the_image, out_image,
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

} /* ends variance */




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
    *       wtiff.c - round_off_image_size
    *                 create_file_if_needed
    *                 write_array_into_tiff_image
    *                 round_off_image_size
    *       tiff.c - read_tiff_header
    *       rtiff.c - read_tiff_image
    *       numcvrt.c - get_integer
    *       edge.c - fix_edges
    *
    *    Modifications:
    *       26 March 1991 - created
    *
    ***********************************************/








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
