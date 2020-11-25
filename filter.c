
    /***********************************************
    *
    *    file filter.c
    *
    *    Functions: This file contains
    *       filter_image
    *       median_filter
    *       high_pixel
    *       low_pixel
    *       setup_filters
    *       get_filter_options
    *       median_of
    *       fsort_elements
    *       fswap
    *
    *    Purpose:
    *       These functions implement several
    *       types of basic spatial frequency
    *       filters.
    *
    *    External Calls:
    *       utility.c - fix_edges
    *
    *    Modifications:
    *       15 February 1992 - created
    *       22 April 1998 - added capability to
    *          work an entire image at one time.
    *
    *************************************************/

#include "cips.h"


int fix_edges();

     /*******************************************
     *
     *   Define the filter masks.
     *
     *******************************************/

short lpf_filter_6[3][3] =
   { {0, 1, 0},
     {1, 2, 1},
     {0, 1, 0}};

short lpf_filter_9[3][3] =
   { {1, 1, 1},
     {1, 1, 1},
     {1, 1, 1}};

short lpf_filter_10[3][3] =
   { {1, 1, 1},
     {1, 2, 1},
     {1, 1, 1}};

short lpf_filter_16[3][3] =
   { {1, 2, 1},
     {2, 4, 2},
     {1, 2, 1}};

short lpf_filter_32[3][3] =
   { {1,  4, 1},
     {4, 12, 4},
     {1,  4, 1}};

short hpf_filter_1[3][3] =
   { { 0, -1,  0},
     {-1,  5, -1},
     { 0, -1,  0}};

short hpf_filter_2[3][3] =
   { {-1, -1, -1},
     {-1,  9, -1},
     {-1, -1, -1}};

short hpf_filter_3[3][3] =
   { { 1, -2,  1},
     {-2,  5, -2},
     { 1, -2,  1}};


         /***********************************************
         *
         *    fswap(...
         *
         *    This function swaps two shorts.
         *
         ***********************************************/

     int fswap(a, b)
        short *a, *b;
     {
        short temp;
        temp  = *a;
        *a    = *b;
        *b    = temp;
     return(1);
     }  /* ends swap */


         /***********************************************
         *
         *    fsort_elements(...
         *
         *    This function performs a simple bubble
         *    sort on the elements from the median
         *    filter.
         *
         ***********************************************/

     int fsort_elements(elements, count)
        int   *count;
        short elements[];
     {
        int i, j;
        j = *count;
        while(j-- > 1){
           for(i=0; i<j; i++){
              if(elements[i] > elements[i+1])
                 fswap(&elements[i], &elements[i+1]);
           }
        }

     return(1);
     }  /* ends fsort_elements */



         /***********************************************
         *
         *    median_of(...
         *
         *    This function finds and returns the
         *    median value of the elements array.
         *
         *    As a side result, it also sorts the
         *    elements array.
         *
         ***********************************************/

     int median_of(elements, count)
        int   *count;
        short elements[];
     {
        short median;

        fsort_elements(elements, count);
        median = elements[*count/2];
        return(median);
     }  /* ends median_of */


         /************************************************
         *
         *    setup_filters(...
         *
         *    This function copies the filter mask
         *    values defined at the top of this file
         *    into the filter array.
         *
         ************************************************/


     int setup_filters(filter_type, low_high, filter)
        char   low_high[];
        int    filter_type;
        short  filter[3][3];
     {
        int i, j;

        if(low_high[0] == 'l'  || low_high[0] =='L'){
           if(filter_type == 6){
              for(i=0; i<3; i++){
                for(j=0; j<3; j++){
                  filter[i][j] = lpf_filter_6[i][j];
                }
              }
           }  /* ends if filter_type == 6 */

           if(filter_type == 9){
              for(i=0; i<3; i++){
                for(j=0; j<3; j++){
                  filter[i][j] = lpf_filter_9[i][j];
                }
              }
           }  /* ends if filter_type == 9 */

           if(filter_type == 10){
              for(i=0; i<3; i++){
                for(j=0; j<3; j++){
                  filter[i][j] = lpf_filter_10[i][j];
                }
              }
           }  /* ends if filter_type == 10 */


           if(filter_type == 16){
              for(i=0; i<3; i++){
                for(j=0; j<3; j++){
                  filter[i][j] = lpf_filter_16[i][j];
                }
              }
           }  /* ends if filter_type == 16 */


           if(filter_type == 32){
              for(i=0; i<3; i++){
                for(j=0; j<3; j++){
                  filter[i][j] = lpf_filter_32[i][j];
                }
              }
           }  /* ends if filter_type == 32 */
        }  /* ends low pass filter */




        if(low_high[0] == 'h'  || low_high[0] =='H'){
           if(filter_type == 1){
              for(i=0; i<3; i++){
                for(j=0; j<3; j++){
                  filter[i][j] = hpf_filter_1[i][j];
                }
              }
           }  /* ends if filter_type == 1 */

           if(filter_type == 2){
              for(i=0; i<3; i++){
                for(j=0; j<3; j++){
                  filter[i][j] = hpf_filter_2[i][j];
                }
              }
           }  /* ends if filter_type == 2 */

           if(filter_type == 3){
              for(i=0; i<3; i++){
                for(j=0; j<3; j++){
                  filter[i][j] = hpf_filter_3[i][j];
                }
              }
           }  /* ends if filter_type == 3 */
        }  /* ends high pass filter */

     return(1);
     }  /* ends setup_filters */



     /*******************************************
     *
     *   filter_image(...
     *
     *   This function filters an image by using
     *   a single 3x3 mask.
     *
     *******************************************/


int filter_image(the_image, out_image,
             rows, cols, bits_per_pixel,
             filter, type, low_high)
   int    type;
   short  filter[3][3],
          **the_image,
          **out_image;
   char   low_high[];
   long   rows, cols, bits_per_pixel;

{
   int    a, b, d, i, j, k,
          length, max, sum, width;

   setup_filters(type, low_high, filter);

   d = type;
   if(type == 2 || type == 3) d = 1;

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
                     filter[a+1][b+1];
            }
         }
         sum               = sum/d;
         if(sum < 0)   sum = 0;
         if(sum > max) sum = max;
         out_image[i][j]   = sum;

      }  /* ends loop over j */
   }  /* ends loop over i */
   fix_edges(out_image, 1, rows-1, cols-1);

return(1);
}  /* ends filter_image */




     /*******************************************
     *
     *   median_filter(..
     *
     *   This function performs a median filter
     *   on an image using a size (3x3, 5x5, etc.)
     *   specified in the call.
     *
     *******************************************/

int median_filter(the_image, out_image,
              rows, cols, size)
   int    size;
   short  **the_image,
          **out_image;
   long   rows, cols;

{
   int    a, b, count, i, j, k,
          length, sd2, sd2p1, ss, width;
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

      /***************************
      *
      *   Loop over image array
      *
      ****************************/

   printf("\n");
   for(i=sd2; i<rows-sd2; i++){
      if( (i%10) == 0) printf("%d ", i);
      for(j=sd2; j<cols-sd2; j++){
         count = 0;
         for(a=-sd2; a<sd2p1; a++){
            for(b=-sd2; b<sd2p1; b++){
               elements[count] = the_image[i+a][j+b];
               count++;
            }
         }
         out_image[i][j] = median_of(elements, &ss);
      }  /* ends loop over j */
   }  /* ends loop over i */

   free(elements);
   fix_edges(out_image, sd2, rows-1, cols-1);

return(1);
}  /* ends median_filter */


     /*******************************************
     *
     *   low_pixel(..
     *
     *   This function replaces the pixel at
     *   the center of a 3x3, 5x5, etc. area
     *   with the min for that area.
     *
     *******************************************/


int low_pixel(the_image, out_image,
          rows, cols, size)
   int    size;
   short  **the_image,
          **out_image;
   long   rows, cols;

{
   int    a, b, count, i, j, k,
          length, sd2, sd2p1, ss, width;
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

      /***************************
      *
      *   Loop over image array
      *
      ****************************/

   printf("\n");
   for(i=sd2; i<rows-sd2; i++){
      if( (i%10) == 0) printf("%d ", i);
      for(j=sd2; j<cols-sd2; j++){
         count = 0;
         for(a=-sd2; a<sd2p1; a++){
            for(b=-sd2; b<sd2p1; b++){
               elements[count] = the_image[i+a][j+b];
               count++;
            }
         }
         fsort_elements(elements, &ss);
         out_image[i][j] = elements[0];
      }  /* ends loop over j */
   }  /* ends loop over i */

   free(elements);
   fix_edges(out_image, sd2, rows-1, cols-1);

return(1);
}  /* ends low_pixel */



     /*******************************************
     *
     *   high_pixel(..
     *
     *   This function replaces the pixel at
     *   the center of a 3x3, 5x5, etc. area
     *   with the max for that area.
     *
     *******************************************/

int high_pixel(the_image, out_image,
           rows, cols, size)
   int    size;
   short  **the_image,
          **out_image;
   long   rows, cols;
{
   int    a, b, count, i, j, k,
          length, sd2, sd2p1, ss, width;
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

      /***************************
      *
      *   Loop over image array
      *
      ****************************/

   printf("\n");
   for(i=sd2; i<rows-sd2; i++){
      if( (i%10) == 0) printf("%d ", i);
      for(j=sd2; j<cols-sd2; j++){
         count = 0;
         for(a=-sd2; a<sd2p1; a++){
            for(b=-sd2; b<sd2p1; b++){
               elements[count] = the_image[i+a][j+b];
               count++;
            }
         }
         fsort_elements(elements, &ss);
         out_image[i][j] = elements[ss-1];
      }  /* ends loop over j */
   }  /* ends loop over i */

   free(elements);
   fix_edges(out_image, sd2, rows-1, cols-1);

return(1);
}  /* ends high_pixel */
