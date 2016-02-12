
   /**************************************************
   *
   *   file hist.c
   *
   *   Functions: This file contains
   *       calculate_histogram
   *       perform_histogram_equalization
   *       zero_histogram
   *       smooth_histogram
   *
   *   Purpose: These functions calculate 
   *      the histogram of an input image array.
   *      They also modify an image by equalizing 
   *      its histogram.
   *
   *   Modifications:
   *       July 86 - ported to IBM-PC
   *       August 1990 - modified for use in the
   *           C Image Processing System
   *       March 1992 - removed the hardwired values
   *           of 100 and replaced them with ROWS
   *           and COLS.  There are still some
   *           hardwired numbers in this file, but
   *           they deal with displaying a histogram.
   *       October 4, 1992 - added the smooth histogram
   *           function.
   *       April 22, 1998 - modified routines to work
   *           with an entire image in one array.
   *
   **************************************************/

#include "cips.h"


#define PRINT_WIDTH  80
#define FORMFEED     '\014'



        /*****************************************
        *
        *   zero_histogram(...
        *
        *   This function clears or zeros a
        *   histogram array.
        *
        ******************************************/

zero_histogram(histogram, gray_levels)
   int      gray_levels;
   unsigned long histogram[];
{
   int i;
   for(i=0; i<gray_levels; i++)
      histogram[i] = 0;
}  /* ends zero_histogram */




        /*****************************************
        *
        *   calculate_histogram(...
        *
        *   This function calculates the histogram
        *   for an input image array.
        *
        ******************************************/

calculate_histogram(image, histogram, length, width)
   int    length, width;
   short  **image;
   unsigned long histogram[];
{
   long  i,j;
   short k;
   for(i=0; i<length; i++){
      for(j=0; j<width; j++){
         k = image[i][j];
         histogram[k] = histogram[k] + 1;
      }
   }
}  /* ends calculate_histogram */




      /********************************************
      *
      *   smooth_histogram(...
      *
      *   This function smoothes the input histogram
      *   and returns it.  It uses a simple averaging
      *   scheme where each point in the histogram
      *   is replaced by the average of itself and
      *   the two points on either side of it.
      *
      *********************************************/

smooth_histogram(histogram, gray_levels)
   int      gray_levels;
   unsigned long histogram[];
{
   int i;
   unsigned long new_hist[gray_levels];

   zero_histogram(new_hist, gray_levels);

   new_hist[0] = (histogram[0] + histogram[1])/2;
   new_hist[gray_levels] =
      (histogram[gray_levels] +
       histogram[gray_levels-1])/2;

   for(i=1; i<gray_levels-1; i++){
      new_hist[i] = (histogram[i-1] +
                     histogram[i]   +
                     histogram[i+1])/3;
   }

   for(i=0; i<gray_levels; i++)
      histogram[i] = new_hist[i];

}  /* ends smooth_histogram */




          /*****************************************
          *
          *    perform_histogram_equalization(...
          *
          *    This function performs histogram
          *    equalization on the input image array.
          *
          ******************************************/

perform_histogram_equalization(image, 
                               histogram,
                               gray_levels,
                               new_grays, 
                               length,
                               width)
   int   gray_levels, new_grays;
   long  length, width;
   short **image;
   unsigned long histogram[];
{
   int i,
       j,
       k;
   unsigned long sum,
            sum_of_h[gray_levels];

   double constant;

   sum = 0;
   for(i=0; i<gray_levels; i++){
      sum         = sum + histogram[i];
      sum_of_h[i] = sum;
   }

      /* constant = new # of gray levels div by area */
   constant = (float)(new_grays)/(float)(length*width);
   for(i=0; i<length; i++){
      for(j=0; j<width; j++){
         k           = image[i][j];
         image[i][j] = sum_of_h[k] * constant;
      }
   }
}  /* ends perform_histogram_equalization */





hist_long_clear_buffer(string)
   char string[];
{
   int i;
   for(i=0; i<300; i++)
      string[i] = ' ';
}
