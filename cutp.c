
    /***********************************************
    *
    *       file cutp.c
    *
    *       Functions: This file contains
    *          paste_image_piece
    *          check_cut_and_paste_limits
    *
    *       Purpose:
    *          These functions paste a part of one
    *          image into another image.
    *
    *       External Calls:
    *          none
    *
    *       Modifications:
    *          3 April 1992 - created
    *         12 August 1998 - modified to work
    *             with an entire image array.
    *
    *************************************************/

#include "cips.h"



     /*******************************************
     *
     *   paste_image_piece(...
     *
     *   This function pastes a rectangular
     *   piece of an image into another image.
     *   The rectangle to be pasted into the image
     *   is described by the il1, ie1, ll1, le1
     *   parameters for the input image.
     *
     *******************************************/

int paste_image_piece(the_image, out_image,
                  il1, ie1, ll1, le1,
                  il2, ie2)
   int    il1, ie1, ll1, le1, il2, ie2;
   short  **the_image,
          **out_image;

{
   int i, j, limit1, limit2;

   limit1 = ll1-il1;
   limit2 = le1-ie1;

   for(i=0; i<limit1; i++){
      for(j=0; j<limit2; j++){
         out_image[il2+i][ie2+j] = the_image[il1+i][ie1+j];
      }
   }
   return(1);
}  /* ends paste_image_piece */



     /*******************************************
     *
     *   check_cut_and_paste_limits(...
     *
     *   This function looks at the line and
     *   element parameters and ensures that they
     *   are not bigger than ROWS and COLS.  If
     *   they are bigger, the last element or
     *   last line parameters are reduced.
     *
     *******************************************/


int check_cut_and_paste_limits(
      il1, ie1,
      ll1, le1,
      il2, ie2,
      image1_length,
      image1_width,
      image2_length,
      image2_width,
      is_ok)
   int il1, ie1, ll1, le1, il2, ie2,
       image1_length, image1_width,
       image2_length, image2_width,
       *is_ok;
{
   int result = 1;

   if(  il1 < 0 ||
        ie1 < 0){
      printf("\nCheck> il1=%d ie1=%d", il1, ie1);
      result = 0;
   }

   if(  il2 < 0 ||
        ie2 < 0){
      printf("\nCheck> il2=%d ie2=%d", il2, ie2);
      result = 0;
   }

   if(ll1 > image1_length){
      printf("\nCheck> ll1=%d length=%d",
         ll1, image1_length);
      result = 0;
   }

   if(le1 > image1_width){
      printf("\nCheck> le1=%d width=%d",
         le1, image1_width);
      result = 0;
   }

   if((il2+(ll1-il1)) > image2_length){
      printf("\nCheck> il2=%d length=%d",
         il2+(ll1-il1), image2_length);
      result = 0;
   }

   if((ie2+(le1-ie1)) > image2_width){
      printf("\nCheck> ie2=%d width=%d",
         ie2+(le1-ie1), image2_width);
      result = 0;
   }

   *is_ok = result;
   return(1);
}  /* ends check_cut_and_paste_limits */
