


       /********************************************
       *
       *       file utility.c
       *
       *       Functions: This file contains
       *          fix_edges
       *          sort_elements
       *          swap
       *
       ********************************************/



#include "cips.h"

int swap();



   /***********************************************
    *
    *    fix_edges(...
    *
    *    This function fixes the edges of an image
    *    array after convolution was performed.
    *    It copies the points near the edge of the
    *    array out to the edge of the array.
    *
    ***********************************************/

int fix_edges(im, w, rows, cols)
      int   w;
      short **im;
      long  rows, cols;
{
   int i, j;

printf("\nFIX> rows=%ld cols=%ld w=%d",rows,cols,w);
      /* four corners */
   for(i=w; i>0; i--){
    im[i-1][i-1] = im[i][i];
    im[i-1][cols-(i-1)] = im[i][cols-1-(i-1)];
    im[rows-(i-1)][i-1] = im[rows-1-(i-1)][i];
    im[rows-(i-1)][cols-(i-1)] = im[rows-1-(i-1)][cols-1-(i-1)];
   }  /* ends four corners loop */

   for(i=0; i<rows; i++){
      for(j=w; j>0; j--){
       im[i][j-1] = im[i][j];
       im[i][cols-j] = im[i][cols-j-1];
      }
   }

   for(j=0; j<cols; j++){
      for(i=w; i>0; i--){
       im[i-1][j] = im[i][j];
       im[rows-i][j] = im[rows-i-1][j];
      }
   }
return(1);
}  /* ends fix_edges */




    /***********************************************
    *
    *    sort_elements(...
    *
    *    This function performs a simple bubble
    *    sort on the elements from the median
    *    filter.
    *
    ***********************************************/

int sort_elements(elements, count)
   int   *count;
   short elements[];
{
   int i, j;
   j = *count;
   while(j-- > 1){
      for(i=0; i<j; i++){
         if(elements[i] > elements[i+1])
            swap(&elements[i], &elements[i+1]);
      }
   }
return(1);
}  /* ends sort_elements */




    /***********************************************
    *
    *    swap(...
    *
    *    This function swaps two shorts.
    *
    ***********************************************/

int swap(a, b)
   short *a, *b;
{
   short temp;
   temp  = *a;
   *a    = *b;
   *b    = temp;
return(1);
}  /* ends swap */
