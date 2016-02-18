
   /*********************************************
   *
   *       file segment.c
   *
   *       Functions: This file contains
   *           adaptive_threshold_segmentation
   *           find_peaks
   *           find_valley_point
   *           grow
   *           insert_into_peaks
   *           insert_into_deltas
   *           label_and_check_neighbors
   *           manual_threshold_segmentation
   *           peak_threshold_segmentation
   *           peaks_high_low
   *           push
   *           pop
   *           is_not_empty
   *           threshold_image_array
   *           valley_high_low
   *           valley_threshold_segmentation
   *
   *       Purpose:
   *           These functions are part of histogram
   *           based image segmentation.
   *
   *       External Calls:
   *           none
   *
   *       Modifications:
   *           October 1992 - created
   *           15 August 1998 - modified to work on 
   *                images at once.
   *           18 February 2016 - with all new
   *                compilers etc. the grow()
   *                isn't working with the linked list
   *                stack, so I revised all that.
   *
   ************************************************/


#include "cips.h"



struct stacks{
   short         x;
   short         y;
   struct stacks *next;
};

struct stacks *stack, *stack1, *tempstack;

void push(short, short);
void pop();
void destroy();

int stackcounter = 0;



   /********************************************
   *
   *  label_and_check_neighbor(...
   *
   *  This function labels a pixel with an object
   *  label and then checks the pixel's 8
   *  neighbors.  If any of the neigbors are
   *  set, then they are also labeled.
   *
   ***********************************************/

int label_and_check_neighbor(binary_image, 
                         g_label, 
                         r, e, value,
                         first_call, 
                         rows, cols)
int   cols, 
      e,
      *first_call,
      r,
      rows;
short **binary_image,
      g_label,
      value;
{
char rr[80];
   int already_labeled = 0,
       i, j;
   struct stacks *temp;

/***printf("\nLACN> start");***/
   if (binary_image[r][e] == g_label)
      already_labeled = 1;

   binary_image[r][e] = g_label;

      /***************************************
      *
      *   Look at the 8 neighors of the
      *   point r,e.
      *
      *   Ensure the points you are checking
      *   are in the image, i.e. not less
      *   than zero and not greater than
      *   rows-1 or cols-1.
      *
      ***************************************/

   for(i=(r-1); i<=(r+1); i++){
      for(j=(e-1); j<=(e+1); j++){

            /********************************
            *
            *   Ensure i and j are not 
            *   outside the boundary of the
            *   image.
            *
            *********************************/

         if((i>=0)   &&
            (i<=rows-1)  &&
            (j>=0)   &&
            (j<=cols-1)){

            if(binary_image[i][j] == value){
               push(i, j);
            }  /* end of if binary_image == value */
         }  /* end if i and j are on the image */
      }  /* ends loop over i rows           */
   }  /* ends loop over j columns        */
   return(1);
}  /* ends label_and_check_neighbors  */





   /**************************************************
   *
   *   threshold_image_array(...
   *
   *   This function thresholds an input image array
   *   and produces a binary output image array.
   *   If the pixel in the input array is between
   *   the hi and low values, then it is set to value.
   *   Otherwise, it is set to 0.
   *
   ***************************************************/


int threshold_image_array(in_image, out_image, 
                      hi, low, value,
                      rows, cols)
   short hi, low, **in_image,
         **out_image, value;
   int   rows, cols;
{
   int   i, j;
   unsigned long counter = 0L;
   for(i=0; i<rows; i++){
      for(j=0; j<cols; j++){
         if(in_image[i][j] >= low  &&
            in_image[i][j] <= hi){
            out_image[i][j] = value;
            counter++;
         }
         else
            out_image[i][j] = 0;
      }  /* ends loop over j */
   }  /* ends loop over i */
   printf("\nTIA> set %ld points\n", counter);
   return(1);
}  /* ends threshold_image_array */





    /**********************************************
    *
    *  grow(...
    *
    *  This function is an object detector.
    *  Its input is an binary image array
    *  containing 0's and value's.
    *  It searches through the image and connects
    *  the adjacent values.
    *
    * FEBRUARY 2016
    * USE %p WHEN PRINTF A POINTER TO A STRUCT
    *
    ***********************************************/

int grow(binary, value, rows, cols)
   short **binary,
         value;
{
   int   first_call,
         i,
         j,
         object_found;
   short g_label,
         pop_i,
         pop_j;


            /*************************************
            *
            *   Now begin the process of growing
            *   regions.
            *
            **************************************/

   g_label       = 2;
   object_found  = 0;
   first_call    = 1;

   for(i=0; i<rows; i++){
      for(j=0; j<cols; j++){
  
            /* This creates the stack */
         stack = NULL;
/***printf("\nGROW> stack is %p",stack);***/

               /**********************************
               *
               *  Search for the first pixel of
               *  a region.
               *
               ***********************************/

         if(binary[i][j] == value){
/***printf("\nGROW> Hit value at %d %d number %d",i,j,g_label);***/
            label_and_check_neighbor(
                      binary, 
                      g_label, 
                      i, j, value, 
                      &first_call, 
                      rows, cols);
            object_found = 1;
         }  /* ends if binary[i]j] == value */

/***printf("\nGROW> stack is %p",stack);***/

               /*****************************
               *
               *  If the stack is not empty,
               *  pop the coordinates of
               *  the pixel off the stack
               *  and check its 8 neighbors.
               *
               *******************************/

         while(is_not_empty()){ /* checks stack */
            pop(&pop_i, &pop_j);
/***if(g_label > 93) printf("\n\t\tGROW> %d",stackcounter);***/
            label_and_check_neighbor(
                        binary,
                        g_label,
                        pop_i,
                        pop_j, value,
                        &first_call,
                        rows, cols);
         }  /* ends while stack_empty == 0 */

         if(object_found == 1){
            object_found = 0;
/***printf("\nGROW> g_label is %d", g_label);***/
            ++g_label;
            destroy();
         }  /* ends if object_found == 1 */

      }   /* ends loop over j */
   }  /* ends loop over i */

   printf("\nGROW> found %d objects", g_label-2);
   return(1);

} /* ends grow  */











   /**************************************************
   *
   *   manual_threshold_segmentation(...
   *
   *   This function segments an image using thresholding
   *   given the hi and low values of the threshold
   *   by the calling routine.  It reads in an image
   *   and writes the result to the output image.
   *
   *   If the segment parameter is 0, you only
   *   threshold the array - you do not segment.
   *
   ***************************************************/

int manual_threshold_segmentation(the_image, out_image,
                              hi, low, value, segment,
                              rows, cols)
   int    rows, cols, segment;
   short  hi, low, **the_image,
          **out_image, value;
{
   threshold_image_array(the_image, out_image,
            hi, low, value, rows, cols);
   if(segment == 1)
      grow(out_image, value, rows, cols);

   return(1);
}  /* ends manual_threshold_segmentation */






   /********************************************
   *
   *   insert_into_peaks(...
   *
   *   This function takes a value and its
   *   place in the histogram and inserts them
   *   into a peaks array.  This helps us rank
   *   the the peaks in the histogram.
   *
   *   The objective is to build a list of
   *   histogram peaks and thier locations.
   *
   *   The peaks array holds the peak value
   *   in the first place and its location in
   *   the second place.
   *
   *********************************************/

int insert_into_peaks(peaks, max, max_place)
   int max, max_place, peaks[PEAKS][2];
{
   int i, j;

      /* first case */
   if(max > peaks[0][0]){
      for(i=PEAKS-1; i>0; i--){
         peaks[i][0] = peaks[i-1][0];
         peaks[i][1] = peaks[i-1][1];
      }
      peaks[0][0] = max;
      peaks[0][1] = max_place;
   }  /* ends if */

      /* middle cases */
   for(j=0; j<PEAKS-3; j++){
      if(max < peaks[j][0]  && max > peaks[j+1][0]){
         for(i=PEAKS-1; i>j+1; i--){
            peaks[i][0] = peaks[i-1][0];
            peaks[i][1] = peaks[i-1][1];
         }
         peaks[j+1][0] = max;
         peaks[j+1][1] = max_place;
      }  /* ends if */
   }  /* ends loop over j */
      /* last case */
   if(max < peaks[PEAKS-2][0]  && 
      max > peaks[PEAKS-1][0]){
      peaks[PEAKS-1][0] = max;
      peaks[PEAKS-1][1] = max_place;
   }  /* ends if */

   return(1);
}  /* ends insert_into_peaks */






   /********************************************
   *
   *   find_peaks(...
   *
   *   This function looks through the histogram
   *   array and finds the two highest peaks.
   *   The peaks must be separated, cannot be
   *   next to each other, by a spacing defined
   *   in cips.h.
   *
   *   The peaks array holds the peak value
   *   in the first place and its location in
   *   the second place.
   *
   *********************************************/

int find_peaks(histogram, peak1, peak2)
   unsigned long histogram[];
   int *peak1, *peak2;
{
   int distance[PEAKS], peaks[PEAKS][2];
   int i, j=0, max=0, max_place=0;

   for(i=0; i<PEAKS; i++){
      distance[i] =  0;
      peaks[i][0] = -1;
      peaks[i][1] = -1;
   }

   for(i=0; i<=GRAY_LEVELS; i++){
      max       = histogram[i];
      max_place = i;
      insert_into_peaks(peaks, max, max_place);
   }  /* ends loop over i */

   for(i=1; i<PEAKS; i++){
      distance[i] = peaks[0][1] - peaks[i][1];
      if(distance[i] < 0)
         distance[i] = distance[i]*(-1);
   }

   *peak1 = peaks[0][1];
   for(i=PEAKS-1; i>0; i--)
    if(distance[i] > PEAK_SPACE) *peak2 = peaks[i][1];

   return(1);
}  /* ends find_peaks */










   /********************************************
   *
   *   peaks_high_low(...
   *
   *   This function uses the histogram array
   *   and the peaks to find the best high and
   *   low threshold values for the threshold
   *   function.  You want the hi and low values
   *   so that you will threshold the image around
   *   the smaller of the two "humps" in the
   *   histogram.  This is because the smaller
   *   hump represents the objects while the
   *   larger hump represents the background.
   *
   *********************************************/

int peaks_high_low(histogram, peak1, peak2, hi, low)
   int  peak1, peak2;
   short *hi, *low;
   unsigned long histogram[];
{
   int i, mid_point;
   unsigned long sum1 = 0, sum2 = 0;

   if(peak1 > peak2)
      mid_point = ((peak1 - peak2)/2) + peak2;
   if(peak1 < peak2)
      mid_point = ((peak2 - peak1)/2) + peak1;

   for(i=0; i<mid_point; i++)
      sum1 = sum1 + histogram[i];

   for(i=mid_point; i<=GRAY_LEVELS; i++)
      sum2 = sum2 + histogram[i];
   if(sum1 >= sum2){
      *low = mid_point;
      *hi  = GRAY_LEVELS;
   }
   else{
      *low = 0;
      *hi  = mid_point;
   }

   return(1);
}  /* ends peaks_high_low */






   /************************************************
   *
   *   peak_threshold_segmentation(...
   *
   *   This function segments an image using
   *   thresholding.  It uses the histogram peaks
   *   to find the hi and low values of the
   *   threshold.
   *
   *   If the segment parameter is 0, you only
   *   threshold the array - you do not segment.
   *
   *************************************************/

int peak_threshold_segmentation(the_image, out_image,
                            value, segment,
                            rows, cols)
   int    rows, cols, segment;
   short  **the_image, **out_image, value;
{
   int      peak1, peak2;
   short    hi, low;
   unsigned long histogram[GRAY_LEVELS+1];

   zero_histogram(histogram, GRAY_LEVELS+1);
   calculate_histogram(the_image, histogram, 
                       rows, cols);
   smooth_histogram(histogram, GRAY_LEVELS+1);
   find_peaks(histogram, &peak1, &peak2);
   peaks_high_low(histogram, peak1, peak2,
                  &hi, &low);
printf("\nPTS> hi=%d low=%d\n", hi, low);
   threshold_image_array(the_image, 
                         out_image,
                         hi, low, value, 
                         rows, cols);
   if(segment == 1)
      grow(out_image, value, rows, cols);

   return(1);
}  /* ends peak_threshold_segmentation */





      /********************************************
      *
      *   valley_high_low(...
      *
      *   This function uses the histogram array
      *   and the valleys to find the best high and
      *   low threshold values for the threshold
      *   function.  You want the hi and low values
      *   so that you will threshold the image around
      *   the smaller of the two "humps" in the
      *   histogram.  This is because the smaller
      *   hump represents the objects while the
      *   larger hump represents the background.
      *
      *********************************************/

int valley_high_low(histogram, peak1, peak2, hi, low)
   int  peak1, peak2;
   short *hi, *low;
   unsigned long histogram[];
{
   int  i, valley_point;
   unsigned long sum1 = 0, sum2 = 0;

   find_valley_point(histogram, peak1, peak2,
                     &valley_point);
   /*printf("\nVHL> valley point is %d", valley_point);*/

   for(i=0; i<valley_point; i++)
      sum1 = sum1 + histogram[i];
   for(i=valley_point; i<=GRAY_LEVELS; i++)
      sum2 = sum2 + histogram[i];

   if(sum1 >= sum2){
      *low = valley_point;
      *hi  = GRAY_LEVELS;
   }
   else{
      *low = 0;
      *hi  = valley_point;
   }

   return(1);
}  /* ends valley_high_low */





   /********************************************
   *
   *   find_valley_point(...
   *
   *   This function finds the low point of
   *   the valley between two peaks in a
   *   histogram.  It starts at the lowest
   *   peak and works its way up to the
   *   highest peak.  Along the way, it looks
   *   at each point in the histogram and inserts
   *   them into a list of points.  When done,
   *   it has the location of the smallest histogram
   *   point - that is the valley point.
   *
   *   The deltas array holds the delta value
   *   in the first place and its location in
   *   the second place.
   *
   *********************************************/

int find_valley_point(histogram, peak1, 
                  peak2, valley_point)
   int  peak1, peak2, *valley_point;
   unsigned long histogram[];
{
   int  deltas[PEAKS][2], delta_hist, i;

   for(i=0; i<PEAKS; i++){
      deltas[i][0] = 10000;
      deltas[i][1] =    -1;
   }

   if(peak1 < peak2){
      for(i=peak1+1; i<peak2; i++){
         delta_hist = (int)(histogram[i]);
         insert_into_deltas(deltas, delta_hist, i);
      }  /* ends loop over i */
   }  /* ends if peak1 < peak2 */

   if(peak2 < peak1){
      for(i=peak2+1; i<peak1; i++){
         delta_hist = (int)(histogram[i]);
         insert_into_deltas(deltas, delta_hist, i);
      }  /* ends loop over i */
   }  /* ends if peak2 < peak1 */

   *valley_point = deltas[0][1];

   return(1);
}  /* ends find_valley_point */







   /********************************************
   *
   *   insert_into_deltas(...
   *
   *   This function inserts histogram deltas
   *   into a deltas array.  The smallest delta
   *   will be at the top of the array.
   *
   *   The objective is to build a list of
   *   histogram area deltas and thier locations.
   *
   *   The deltas array holds the delta value
   *   in the first place and its location in
   *   the second place.
   *
   *********************************************/

int insert_into_deltas(deltas, value, place)
   int value, place, deltas[PEAKS][2];
{
   int i, j;

      /* first case */
   if(value < deltas[0][0]){
      for(i=PEAKS-1; i>0; i--){
         deltas[i][0] = deltas[i-1][0];
         deltas[i][1] = deltas[i-1][1];
      }
      deltas[0][0] = value;
      deltas[0][1] = place;
   }  /* ends if */

      /* middle cases */
   for(j=0; j<PEAKS-3; j++){
      if(value > deltas[j][0]  &&
         value < deltas[j+1][0]){
         for(i=PEAKS-1; i>j+1; i--){
            deltas[i][0] = deltas[i-1][0];
            deltas[i][1] = deltas[i-1][1];
         }
         deltas[j+1][0] = value;
         deltas[j+1][1] = place;
      }  /* ends if */
   }  /* ends loop over j */

      /* last case */
   if(value > deltas[PEAKS-2][0]  &&
      value < deltas[PEAKS-1][0]){
      deltas[PEAKS-1][0] = value;
      deltas[PEAKS-1][1] = place;
   }  /* ends if */

   return(1);
}  /* ends insert_into_deltas */
















   /************************************************
   *
   *   valley_threshold_segmentation(...
   *
   *   This function segments an image using
   *   thresholding.  It uses the histogram valleys
   *   to find the hi and low values of the
   *   threshold.
   *
   *   If the segment parameter is 0, you only
   *   threshold the array - you do not segment.
   *
   *************************************************/

int valley_threshold_segmentation(the_image, out_image,
                              value, segment,
                              rows, cols)
   int    rows, cols, segment;
   short  **the_image,
          **out_image, value;
{
   int      peak1, peak2;
   short    hi, low;
   unsigned long histogram[GRAY_LEVELS+1];

   zero_histogram(histogram, GRAY_LEVELS+1);
   calculate_histogram(the_image, histogram, rows, cols);
   smooth_histogram(histogram, GRAY_LEVELS+1);
   find_peaks(histogram, &peak1, &peak2);
   valley_high_low(histogram, peak1, peak2,
                   &hi, &low);
   threshold_image_array(the_image, out_image,
                         hi, low, value, rows, cols);
   if(segment == 1)
      grow(out_image, value, rows, cols);

   return(1);
}  /* ends valley_threshold_segmentation */






   /**************************************************
   *
   *   threshold_and_find_means(...
   *
   *   This function thresholds an input image array
   *   and produces a binary output image array.
   *   If the pixel in the input array is between
   *   the hi and low values, then it is set to value.
   *   Otherwise, it is set to 0.
   *
   ***************************************************/

int threshold_and_find_means(in_image, out_image, hi,
                         low, value, object_mean,
                         background_mean,
                         rows, cols)
   short *background_mean, hi, low,
         **in_image, *object_mean,
         **out_image, value;
   int   rows, cols;
{
   int      counter = 0,
            i,
            j;
   unsigned long object     = 0,
                 background = 0;

   for(i=0; i<rows; i++){
      for(j=0; j<cols; j++){
         if(in_image[i][j] >= low  &&
            in_image[i][j] <= hi){
            out_image[i][j] = value;
            counter++;
            object = object + in_image[i][j];
         }
         else{
            out_image[i][j] = 0;
            background      = background + in_image[i][j];
         }
      }  /* ends loop over j */
   }  /* ends loop over i */
   object     = object/counter;
   background = background/((rows*cols)-counter);
   *object_mean     = (short)(object);
   *background_mean = (short)(background);
   printf("\n\tTAFM> set %d points", counter);
   printf("\n\tTAFM> object=%d background=%d",
          *object_mean, *background_mean);
   return(1);
}  /* ends threshold_and_find_means */







   /************************************************
   *
   *   adaptive_threshold_segmentation(...
   *
   *   This function segments an image using
   *   thresholding.  It uses two passes
   *   to find the hi and low values of the
   *   threshold.  The first pass uses the peaks
   *   of the histogram to find the hi and low
   *   threshold values.  It thresholds the image
   *   using these hi lows and calculates the means
   *   of the object and background.  Then we use
   *   these means as new peaks to calculate new
   *   hi and low values.  Finally, we threshold
   *   the image again using these second hi low
   *   hi low values.
   *
   *   If the segment parameter is 0, you only
   *   threshold the array - you do not segment.
   *
   *************************************************/

int adaptive_threshold_segmentation(the_image, out_image,
                                value, segment,
                                rows, cols)
   int    rows, cols, segment;
   short  **the_image,
          **out_image, value;
{
   int      peak1, peak2;
   short    background, hi, low, object;
   unsigned long histogram[GRAY_LEVELS+1];

   zero_histogram(histogram, GRAY_LEVELS+1);
   calculate_histogram(the_image, histogram, 
                       rows, cols);
   smooth_histogram(histogram, GRAY_LEVELS+1);
   find_peaks(histogram, &peak1, &peak2);
   peaks_high_low(histogram, peak1, peak2,
                  &hi, &low);
   threshold_and_find_means(the_image, out_image,
                            hi, low, value,
                            &object, &background,
                            rows, cols);
   peaks_high_low(histogram, object, background,
                  &hi, &low);
printf("\nATS> hi=%d low=%d\n",hi,low);
   threshold_image_array(the_image, out_image,
                         hi, low, value,
                         rows, cols);
   if(segment == 1)
      grow(out_image, value, rows, cols);

   return(1);
}  /* ends adaptive_threshold_segmentation */




/*** U T I L I T I E S ***/

void show_stack()
{
char r[80];
   stack1 = stack;
   while(stack1 != NULL){
      printf("\n\t\t\t\t%d %d %p",
      stack1->x,stack1->y, stack1->next);
      stack1 = stack1->next;
   }
}
   /***************************/
int is_not_empty()
{
   int result = 0;
   if(stack != NULL)
      result = 1;
   return(result);
}  /* ends is_empty */

   /***************************/
void push(x, y)
   short  x, y;
{
   char r[80];

   if(stack == NULL){
      stack = (struct stacks *) malloc(sizeof(struct stacks ));
      stack->x    = x;
      stack->y    = y;
      stack->next = NULL;
   }  /* ends if */
   else{
      tempstack = (struct stacks *) malloc(sizeof(struct stacks ));
      tempstack->x    = x;
      tempstack->y    = y;
      tempstack->next = stack;
      stack           = tempstack; 
   }  /* ends else */
   stackcounter++;
}  /* ends push */


   /***************************/
void pop(x, y)
   short  *x, *y;
{

   stack1 = stack;
   if(stack1 == NULL){
      printf("\nPOP ERROR - empty stack\n");
      return;
   }  /* ends if */
   else{
      *x     = stack->x;
      *y     = stack->y;
      stack1 = stack1->next;
      free(stack);
      stack = stack1;
      stackcounter--;
   }  /* ends else */
}  /* ends pop */

   /***************************/
void destroy()
{
   stack1 = stack;
   while(stack1 != NULL){
      stack1 = stack->next;
      free(stack);
      stack  = stack1;
      stack1 = stack1->next;
   }  /* ends while */
   free(stack1);
   stack = NULL;
}  /* ends destroy */



/*** E N D   O F   U T I L I T I E S ***/



