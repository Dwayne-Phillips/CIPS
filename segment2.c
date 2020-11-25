

       /***********************************************
       *
       *       file segment2.c
       *
       *       Functions: This file contains
       *          find_cutoff_point
       *          edge_region
       *          gray_shade_region
       *          edge_gray_shade_region
       *          pixel_grow
       *          pixel_label_and_check_neighbors
       *          is_close
       *          erode_image_array
       *          get_edge_region_options
       *
       *       Purpose:
       *          These function implement the three
       *          segmentation techniques in Image
       *          Processing part 10.
       *
       *       External Calls:
       *          edges.c - quick_edge
       *                    homogeneity
       *                    difference_edge
       *                    contrast_edge
       *                    gaussian_edge
       *                    range
       *                    variance
       *                    detect_edges
       *          hist.c - calculate_histogram
       *                   zero_histogram
       *          thresh.c - threshold_image_array
       *
       *       Modifications:
       *          5 December 1992 - created
       *         15 August 1998 - modified to work on entire
       *              images at once.
       *
       *************************************************/

#include "cips.h"



struct stacksp{
   short         x;
   short         y;
   struct stacksp *next;
};

struct stacksp *stackp;

int is_not_emptyp();
int pushp();
int popp();
int is_close();
int detect_type();
int detect_edges();
int quick_edge();
int homogeneity();
int difference_edge();
int contrast_edge();
int gaussian_edge();
int range();
int variance();
int zero_histogram();
int calculate_histogram();
int threshold_image_array();
int read_image_array();




   /********************************************
   *
   *  pixel_label_and_check_neighbors(...
   *
   *  This function labels a pixel with an object
   *  label and then checks the pixel's 8
   *  neighbors.  If any of the neigbors are
   *  set, then they are also labeled.
   *
   *  It also updates the target or ave pixel
   *  value of the pixels in the region being
   *  grown.
   *
   ***********************************************/

int pixel_label_and_check_neighbor(input_image,
                         output_image, target,
                         sum, count,
                         g_label,
                         r, e, diff,
                         first_call,
                         rows, cols)
int   *count,
      e,
      *first_call,
      r;
long  cols, rows;
short **input_image,
      **output_image,
      g_label,
      *sum,
      *target,
      diff;
{
char response[80];
   int already_labeled = 0,
       i, j;

/**printf("\nDEBUG>placn> start rx=%d ey=%d",r,e);**/
   if (output_image[r][e] != 0)
      already_labeled = 1;

   output_image[r][e] = g_label;
   *count  = *count + 1;
   if(*count > 1){
      *sum    = *sum + input_image[r][e];
      *target = *sum / *count;
   }

      /***************************************
      *
      *   Look at the 8 neighors of the
      *   point r,e.
      *
      *   Ensure the points are close enough
      *   to the target and do not equal
      *   FORGET_IT.
      *
      *   Ensure the points you are checking
      *   are in the image, i.e. not less
      *   than zero and not greater than
      *   rows-1 or cols-1.
      *
      ***************************************/

   for(i=(r-1); i<=(r+1); i++){
      for(j=(e-1); j<=(e+1); j++){

         if((i>=0)       &&  /* ensure point in in image */
            (i<=rows-1)  &&
            (j>=0)       &&
            (j<=cols-1)){

            if( input_image[i][j] != FORGET_IT   &&
                is_close(input_image[i][j],
                            *target, diff)       &&
                output_image[i][j] == 0){

               pushp(i, j);

            }  /* ends if is_close */
         }  /* end if i and j are on the image */
      }  /* ends loop over i rows           */
   }  /* ends loop over j columns        */

return(1);
}  /* ends pixel_label_and_check_neighbors  */






   /********************************************
   *
   *  is_close(...
   *
   *  This function tests to see if two pixel
   *  values are close enough together.  It
   *  uses the delta parameter to make this
   *  judgement.
   *
   ***********************************************/

int is_close(a, b, delta)
   short a, b, delta;
{
   int   result = 0;
   short diff;

   diff = a-b;
   if(diff < 0) diff = diff*(-1);
   if(diff < delta)
      result = 1;
   return(result);
}  /* ends is_close */



     /*******************************************
     *
     *   find_cutoff_point(..
     *
     *   This function looks at a histogram
     *   and sets a cuttoff point at a given
     *   percentage of pixels.
     *   For example, if percent=0.6, you
     *   start at 0 in the histogram and count
     *   up until you've hit 60% of the pixels.
     *   Then you stop and return that pixel
     *   value.
     *
     ********************************************/

int find_cutoff_point(histogram, percent,
                  cutoff, rows, cols)
   unsigned long histogram[];
   float    percent;
   long     cols, rows;
   short    *cutoff;
{
   float  fd, fsum, sum_div;
   int    i, looking;
   long   lc, lr, num=0, sum=0;

   sum     = 0;
   i       = 0;
   lr      = (long)(rows);
   lc      = (long)(cols);
   num     = lr*lc;
   fd      = (float)(num);

   while(looking){
      fsum    = (float)(sum);
      sum_div = fsum/fd;
      if(sum_div >= percent)
         looking = 0;
      else
         sum = sum + histogram[i++];
   }  /* ends while looking */

   if(i >= (GRAY_LEVELS+1)) i = GRAY_LEVELS;
   *cutoff = i;
   printf("\nCutoff is %d sum=%ld", *cutoff, sum);
return(1);
}  /* ends find_cutoff_point */




     /*******************************************
     *
     *   erode_image_array(..
     *
     *   This function erodes pixels.  If a pixel
     *   equals value and has more than threshold
     *   neighbors equal to 0, then set that
     *   pixel in the output to 0.
     *
     *******************************************/


int erode_image_array(the_image, out_image,
                  value, threshold,
                  rows, cols)
   short  **the_image,
          **out_image,
          threshold,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k,
          length, width;

      /***************************
      *
      *   Loop over image array
      *
      ****************************/

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = the_image[i][j];

   printf("\n");

   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%3d", i);
      for(j=1; j<cols-1; j++){
         if(the_image[i][j] == value){
            count = 0;
            for(a=-1; a<=1; a++){
                for(b=-1; b<=1; b++){
                      if(the_image[i+a][j+b] == 0)
                         count++;
                }  /*  ends loop over b */
            }  /* ends loop over a */
            if(count > threshold) out_image[i][j] = 0;
         }  /* ends if the_image == value */
      }  /* ends loop over j */
   }  /* ends loop over i */

return(1);
}  /* ends erode_image_array */




    /**********************************************
    *
    *   pixel_grow(...
    *
    *   The function grows regions.  It is similar
    *   to the grow function in segment.c, but it
    *   has several new capabilities.  It can
    *   eliminate regions if they are too large or
    *   too small.
    *
    *   It ignores pixels = FORGET_IT.  This allows
    *   it to ignore edges or regions already
    *   eliminated from consideration.
    *
    *   It adds pixels to a growing region only if
    *   the pixel is close enough to the average gray
    *   level of that region.
    *
    ***********************************************/

int pixel_grow(input, output, diff,
           min_area, max_area,
           rows, cols)
   long  cols, rows;
   short **input,
         **output,
         max_area,
         min_area,
         diff;
{
   char name[80];

   int count,
       first_call,
       i,
       ii,
       j,
       jj,
       object_found;

   short g_label, target, pop_i, pop_j, sum;

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         output[i][j] = 0;

   g_label       = 2;
   object_found  = 0;
   first_call    = 1;

            /*************************************
            *
            *   Now begin the process of growing
            *   regions.
            *
            **************************************/

   for(i=0; i<rows; i++){
if( (i%4) == 0) printf("\n");
printf("-i=%3d label=%3d", i, g_label);
      for(j=0; j<cols; j++){

         target            = input[i][j];
         sum               = target;
         count             = 0;
         /********************************
         stack_file_in_use = 0;
         stack_empty       = 1;
         pointer           = -1;
         *******************************/
         stackp = NULL;

               /**********************************
               *
               *  Search for the first pixel of
               *  a region.  It must not equal
               *  FORGET_IT, and it must be close
               *  enough to the target (ave value).
               *
               ***********************************/

         if(input[i][j] != FORGET_IT            &&
            is_close(input[i][j], target, diff) &&
            output[i][j] == 0){
            pixel_label_and_check_neighbor(input,
                           output, &target, &sum,
                           &count, g_label,
                           i, j, diff,
                           &first_call,
                           rows, cols);
            object_found = 1;
         }  /* ends if is_close */

               /*****************************
               *
               *  If the stack is not empty,
               *  pop the coordinates of
               *  the pixel off the stack
               *  and check its 8 neighbors.
               *
               *******************************/

         while(is_not_emptyp(stackp)){
            popp(&pop_i, &pop_j);


            pixel_label_and_check_neighbor(input,
                           output, &target, &sum,
                           &count, g_label,
                           pop_i, pop_j,
                           diff,
                           &first_call,
                           rows, cols);
         }  /* ends while stack_empty == 0 */

         if(object_found == 1){
            object_found = 0;

                  /**********************************
                  *
                  *  The object must be in the
                  *  size constraints given by
                  *  min_area and max_area
                  *
                  *********************************/

            if(count >= min_area  &&
               count <= max_area)
               ++g_label;
                  /**********************************
                  *
                  *   Remove the object from the
                  *   output.  Set all pixels in the
                  *   object you are removing to
                  *   FORGET_IT.
                  *
                  **********************************/

            else{
               for(ii=0; ii<rows; ii++){
                  for(jj=0; jj<cols; jj++){
                     if(output[ii][jj] == g_label){
                        output[ii][jj] = 0;
                        input[ii][jj]  = FORGET_IT;
                     }  /* ends if output == g_label */
                  }  /* ends loop over jj */
               }  /* ends loop over ii */
            }  /* ends else remove object */
         }  /* ends if object_found == 1 */

      }   /* ends loop over j */
   }  /* ends loop over i */

   printf("\nGROW> found %d objects", g_label);
return(1);
} /* ends pixel_grow  */






     /*******************************************
     *
     *   edge_region(..
     *
     *   This function segments an image by
     *   growing regions inside of edges.
     *   The steps are:
     *      . detect edges
     *      . threshold edge output to a
     *        percent value
     *      . remove edges from consideration
     *      . grow regions
     *
     *******************************************/


int edge_region(the_image, out_image,
            edge_type, min_area,
            max_area, diff, percent, set_value,
            erode, rows, cols, bits_per_pixel)
   float    percent;
   int      edge_type;
   long     bits_per_pixel, cols, rows;
   short    diff, erode,
            max_area, min_area,
            set_value,
            **the_image,
            **out_image;
{

   int    a, b, count, i, j, k,
          length, width;
   short  cutoff;
   unsigned long histogram[GRAY_LEVELS+1];

      /***************************
      *
      *   Detect the edges.  Do
      *   not threshold.
      *
      ****************************/

   if(edge_type == 1  ||
      edge_type == 2  ||
      edge_type == 3)
      detect_edges(the_image, out_image,
                   edge_type, 0, 0,
                   rows, cols,
                   bits_per_pixel);

   if(edge_type == 4){
      quick_edge(the_image, out_image,
                 0, 0,
                 rows, cols,
                 bits_per_pixel);
   }  /* ends if 4 */

   if(edge_type == 5){
      homogeneity(the_image, out_image,
                  rows, cols,
                  bits_per_pixel,
                  0, 0);
   }  /* ends if 5 */

   if(edge_type == 6){
      difference_edge(the_image, out_image,
                      rows, cols,
                      bits_per_pixel,
                      0, 0);
   }  /* ends if 6 */

   if(edge_type == 7){
      contrast_edge(the_image, out_image,
                    rows, cols,
                    bits_per_pixel,
                    0, 0);
   }  /* ends if 7 */

   if(edge_type == 8){
      gaussian_edge(the_image, out_image,
                    rows, cols,
                    bits_per_pixel,
                    3, 0, 0);
   }  /* ends if 8 */

   if(edge_type == 10){
      range(the_image, out_image,
            rows, cols,
            bits_per_pixel,
            3, 0, 0);
   }  /* ends if 10 */

   if(edge_type == 11){
      variance(the_image, out_image,
               rows, cols,
               bits_per_pixel,
               0, 0);
   }  /* ends if 11 */

/**write_array_into_tiff_image("f:e1.tif", out_image,
il, ie, ll, le);**/

      /* copy out_image to the_image */
   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         the_image[i][j] = out_image[i][j];

      /******************************
      *
      *   Threshold the edge detector
      *   output at a given percent.
      *   This eliminates the weak
      *   edges.
      *
      *******************************/
   zero_histogram(histogram, GRAY_LEVELS+1);
   calculate_histogram(the_image, histogram,
                       rows, cols);
   find_cutoff_point(histogram, percent, &cutoff,
                     rows, cols);
   threshold_image_array(the_image, out_image,
                         GRAY_LEVELS, cutoff,
                         set_value, rows, cols);

   if(erode != 0){
         /* copy out_image to the_image */
      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];
      erode_image_array(the_image, out_image,
                        set_value, erode,
                        rows, cols);
   }  /* ends if erode */


      /*******************************
      *
      *   Set all the edge values to
      *   FORGET_IT so the region
      *   growing will not use those
      *   points.
      *
      *******************************/

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         if(out_image[i][j] == set_value)
            out_image[i][j] = FORGET_IT;

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         the_image[i][j] = out_image[i][j];

   pixel_grow(the_image, out_image, diff,
              min_area, max_area,
              rows, cols);
return(1);
}  /* ends edge_region */



     /*******************************************
     *
     *   gray_shade_region(...
     *
     *   This function segments an image by
     *   growing regions based only on gray
     *   shade.
     *
     *******************************************/


int gray_shade_region(the_image, out_image,
                  diff, min_area, max_area,
                  rows, cols)
   long   cols, rows;
   short  **the_image,
          **out_image,
          diff, min_area, max_area;
{
   int    a, b, big_count, count, i, j, k, l,
          not_finished, length, width;
printf("\nDEBUG> GSR> before calling pixel grow");
   pixel_grow(the_image, out_image, diff,
              min_area, max_area,
              rows, cols);
printf("\nDEBUG> GSR> after calling pixel grow");
return(1);
}  /* ends gray_shade_region */





     /*******************************************
     *
     *   edge_gray_shade_region(..
     *
     *   This function segments an image by
     *   growing gray shade regions inside of
     *   edges.  It combines the techniques
     *   of the edge_region and gray_shade_region
     *   functions.
     *
     *   The steps are:
     *      . detect edges
     *      . threshold edge output to a
     *        percent value
     *      . lay the edges on top of the original
     *        image to eliminate them from
     *        consideration
     *      . grow regions
     *
     *******************************************/

int edge_gray_shade_region(in_name, the_image,
            out_image, edge_type,
            min_area, max_area, diff, percent,
            set_value, erode,
            rows, cols, bits_per_pixel)
   char     in_name[];
   float    percent;
   int      edge_type;
   long     bits_per_pixel, cols, rows;
   short    diff, erode,
            max_area, min_area,
            set_value,
            **the_image,
            **out_image;
{
   int    a, b, count, i, j, k,
          length, width;
   short  cutoff;
   unsigned long histogram[GRAY_LEVELS+1];

      /***************************
      *
      *   Detect the edges.  Do
      *   not threshold.
      *
      ****************************/

   if(edge_type == 1  ||
      edge_type == 2  ||
      edge_type == 3)
      detect_edges(the_image, out_image,
                   edge_type, 0, 0,
                   rows, cols,
                   bits_per_pixel);

   if(edge_type == 4){
      quick_edge(the_image, out_image,
                 0, 0,
                 rows, cols,
                 bits_per_pixel);
   }  /* ends if 4 */
   if(edge_type == 5){
      homogeneity(the_image, out_image,
                  rows, cols,
                  bits_per_pixel,
                  0, 0);
   }  /* ends if 5 */

   if(edge_type == 6){
      difference_edge(the_image, out_image,
                      rows, cols,
                      bits_per_pixel,
                      0, 0);
   }  /* ends if 6 */

   if(edge_type == 7){
      contrast_edge(the_image, out_image,
                    rows, cols,
                    bits_per_pixel,
                    0, 0);
   }  /* ends if 7 */

   if(edge_type == 8){
      gaussian_edge(the_image, out_image,
                    rows, cols,
                    bits_per_pixel,
                    3, 0, 0);
   }  /* ends if 8 */

   if(edge_type == 10){
      range(the_image, out_image,
            rows, cols,
            bits_per_pixel,
            3, 0, 0);
   }  /* ends if 10 */

   if(edge_type == 11){
      variance(the_image, out_image,
               rows, cols,
               bits_per_pixel,
               0, 0);
   }  /* ends if 11 */

/**write_array_into_tiff_image("f:e1.tif", out_image,
il, ie, ll, le);**/

      /* copy out_image to the_image */
   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         the_image[i][j] = out_image[i][j];

      /******************************
      *
      *   Threshold the edge detector
      *   output at a given percent.
      *   This eliminates the weak
      *   edges.
      *
      *******************************/

   zero_histogram(histogram, GRAY_LEVELS+1);
   calculate_histogram(the_image, histogram,
                       rows, cols);
   find_cutoff_point(histogram, percent, &cutoff,
                     rows, cols);
   threshold_image_array(the_image, out_image,
                         GRAY_LEVELS, cutoff,
                         set_value, rows, cols);


   if(erode != 0){
         /* copy out_image to the_image */
      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];
      erode_image_array(the_image, out_image,
                        set_value, erode,
                        rows, cols);
   }  /* ends if erode */

      /*******************************
      *
      *   Read the original gray shade
      *   image back into the_image.
      *
      *******************************/

   read_image_array(in_name, the_image);

      /*******************************
      *
      *   Overlay the edge values
      *   on top of the original
      *   image by setting them to
      *   FORGET_IT so the region
      *   growing will not use those
      *   points.
      *
      *******************************/

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         if(out_image[i][j] == set_value)
            the_image[i][j] = FORGET_IT;

/**write_array_into_tiff_image("f:e4.tif", the_image,
il, ie, ll, le);**/

   pixel_grow(the_image, out_image, diff,
              min_area, max_area,
              rows, cols);

return(1);
}  /* ends edge_gray_shade_region */






int show_stackp()
{
char r[80];
   struct stacksp *temp;
   temp = stackp;
   while(temp != NULL){
      /**printf("\n\t\t\t\t%d %d %x %x",temp->x,temp->y, temp, temp->next);**/
      temp = temp->next;
   }
return(1);
}



int is_not_emptyp(pointer)
   struct stacksp *pointer;
{
   int result = 0;
   if(pointer != NULL)
      result = 1;
   return(result);

}  /* ends is_empty */



int pushp(x, y)
   short  x, y;
{
   struct stacksp *new_one;

   new_one = (struct stacksp *)
             calloc(1, sizeof(struct stacksp ));
   new_one->next = stackp;
   new_one->x    = x;
   new_one->y    = y;
   stackp        = new_one;
return(1);
}  /* ends push */



int popp(x, y)
   short  *x, *y;
{
   struct stacksp *temp;

   temp     = stackp;
   *x       = stackp->x;
   *y       = stackp->y;
   stackp   = stackp->next;
   free(temp);
return(1);
}  /* ends pop */
