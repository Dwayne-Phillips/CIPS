
       /**********************************************
       *
       *       file skeleton.c
       *
       *       Functions: This file contains
       *          thinning
       *          can_thin
       *          dilate_not_join
       *          can_dilate
       *          little_label_and_check
       *          special_closing
       *          special_opening
       *          edm
       *          distanc_8
       *          mat
       *          mat_d
       *
       *       Purpose:
       *          These functions thin objects in
       *          an image, dilate objects, and
       *          perform opening and closing
       *          without removing or joining
       *          objects.
       *
       *       External Calls:
       *          none
       *
       *
       *       Modifications:
       *          7 March 1993 - created
       *         21 August 1998 - modified to work on entire
       *              images at once.
       *
       ************************************************/

#include "cips.h"

int thinning();
int dilation();
int can_thin();
int can_dilate();
int dilate_not_join();
int erosion();
int little_label_and_check();
int distance_8();
int mat_d();



     /*******************************************
     *
     *   special_opening(...
     *
     *   Opening is erosion followed by dilation.
     *   This routine will use the thinning
     *   erosion routine.  This will not allow
     *   an object to erode to nothing.
     *
     *   The number parameter specifies how
     *   erosions to perform before doing one
     *   dilation.
     *
     *******************************************/

int special_opening(the_image, out_image,
                value, threshold, number,
                rows, cols)
   int    number;
   short  **the_image,
          **out_image,
          threshold, value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;

   thinning(the_image, out_image,
            value, threshold, 1,
            rows, cols);

   if(number > 1){
      count = 1;
      while(count < number){
         count++;
         thinning(the_image, out_image,
                  value, threshold, 1,
                  rows, cols);
      }  /* ends while */
   }  /* ends if number > 1 */

   dilation(the_image, out_image,
            value, threshold,
            rows, cols);
return(1);
}  /* ends special_opening */






     /*******************************************
     *
     *   thinning(...
     *
     *   Use a variation of the grass fire
     *   wave front approach.
     *
     *   Raster scan the image left to right
     *   and examine and thin the left edge pixels
     *   (a 0 to value transition).  Process them
     *   normally and "save" the result.  Next,
     *   raster scan the image right to left and
     *   save.  Raster scan top to bottom and save.
     *   Raster scan bottom to top and save.
     *
     *   That is one complete pass.
     *
     *   Keep track of pixels thinned for a
     *   pass and quit when you make a complete
     *   pass without thinning any pixels.
     *
     *******************************************/


int thinning(the_image, out_image,
         value, threshold, once_only,
         rows, cols)
   int    once_only;
   short  **the_image,
          **out_image,
          threshold, value;
   long   cols, rows;
{
   int    a, b, big_count, count, i, j, k,
          not_finished;

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = the_image[i][j];

   not_finished = 1;
   while(not_finished){

      if(once_only == 1)
        not_finished = 0;
      big_count = 0;

         /***************************
         *
         *   Scan left to right
         *   Look for 0-value transition
         *
         ****************************/

      printf("\n");
      for(i=1; i<rows-1; i++){
         if( (i%10) == 0) printf("%3d", i);
         for(j=1; j<cols-1; j++){
            if(the_image[i][j-1] == 0   &&
               the_image[i][j]   == value){
               count = 0;
               for(a=-1; a<=1; a++){
                   for(b=-1; b<=1; b++){
                         if(the_image[i+a][j+b] == 0)
                            count++;
                   }  /*  ends loop over b */
               }  /* ends loop over a */
               if(count > threshold){
                  if(can_thin(the_image, i, j, value)){
                     out_image[i][j] = 0;
                     big_count++;
                  }  /* ends if can_thin */
               }  /* ends if count > threshold */
            }  /* ends if the_image == value */
         }  /* ends loop over j */
      }  /* ends loop over i */

         /**************************************
         *
         *   Copy the output back to the input.
         *
         **************************************/

      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];


         /***************************
         *
         *   Scan right to left
         *   Do this by scanning left
         *   to right and look for
         *   value-0 transition.
         *
         ****************************/

      printf("\n");
      for(i=1; i<rows-1; i++){
         if( (i%10) == 0) printf("%3d", i);
         for(j=1; j<cols-1; j++){
            if(the_image[i][j+1] == 0   &&
               the_image[i][j]   == value){
               count = 0;
               for(a=-1; a<=1; a++){
                   for(b=-1; b<=1; b++){
                         if(the_image[i+a][j+b] == 0)
                            count++;
                   }  /*  ends loop over b */
               }  /* ends loop over a */
               if(count > threshold){
                  if(can_thin(the_image, i, j, value)){
                     out_image[i][j] = 0;
                     big_count++;
                  }  /* ends if can_thin */
               }  /* ends if count > threshold */
            }  /* ends if the_image == value */
         }  /* ends loop over j */
      }  /* ends loop over i */

         /**************************************
         *
         *   Copy the output back to the input.
         *
         **************************************/

      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];


         /***************************
         *
         *   Scan top to bottom
         *   Look for 0-value transition
         *
         ****************************/

      printf("\n");
      for(j=1; j<cols-1; j++){
         if( (j%10) == 0) printf("%3d", j);
         for(i=1; i<rows-1; i++){
            if(the_image[i-1][j] == 0   &&
               the_image[i][j]   == value){
               count = 0;
               for(a=-1; a<=1; a++){
                   for(b=-1; b<=1; b++){
                         if(the_image[i+a][j+b] == 0)
                            count++;
                   }  /*  ends loop over b */
               }  /* ends loop over a */
               if(count > threshold){
                  if(can_thin(the_image, i, j, value)){
                     out_image[i][j] = 0;
                     big_count++;
                  }  /* ends if can_thin */
               }  /* ends if count > threshold */
            }  /* ends if the_image == value */
         }  /* ends loop over i */
      }  /* ends loop over j */

         /**************************************
         *
         *   Copy the output back to the input.
         *
         **************************************/

      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];



         /***************************
         *
         *   Scan bottom to top
         *   Do this by scanning top
         *   to bottom and look for
         *   value-0 transition.
         *
         ****************************/

      printf("\n");
      for(j=1; j<cols-1; j++){
         if( (j%10) == 0) printf("%3d", j);
         for(i=1; i<rows-1; i++){
            if(the_image[i+1][j] == 0   &&
               the_image[i][j]   == value){
               count = 0;
               for(a=-1; a<=1; a++){
                   for(b=-1; b<=1; b++){
                         if(the_image[i+a][j+b] == 0)
                            count++;
                   }  /*  ends loop over b */
               }  /* ends loop over a */
               if(count > threshold){
                  if(can_thin(the_image, i, j, value)){
                     out_image[i][j] = 0;
                     big_count++;
                  }  /* ends if can_thin */
               }  /* ends if count > threshold */
            }  /* ends if the_image == value */
         }  /* ends loop over i */
      }  /* ends loop over j */

         /**************************************
         *
         *   Copy the output back to the input.
         *
         **************************************/

      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];

         /**************************************
         *
         *   Now look at the result of this big
         *   pass through the image.
         *
         ***************************************/

      printf("\n\nThinned %d pixels", big_count);
      if(big_count == 0)
         not_finished = 0;
      else{
         for(i=0; i<rows; i++)
            for(j=0; j<cols; j++)
               the_image[i][j] = out_image[i][j];
      }  /* ends else */

   }  /* ends while not_finished */

   /****
   fix_edges(out_image, 3, rows, cols);
   *****/
return(1);
}  /* ends thinning */





     /*******************************************
     *
     *   can_thin(...
     *
     *   Look at the neighbors of the center pixel.
     *   If a neighbor == value, then it must
     *   have a neighbor == value other than the
     *   center pixel.
     *
     *   Procedure:
     *   . Copy the 3x3 area surrounding pixel
     *     i,j to a temp array.
     *   . Set the center pixel to zero.
     *   . Look at each non-zero pixel in temp.
     *      . If you cannot find a non-zero
     *        neighbor.
     *      . Then you cannot thin.
     *
     *******************************************/

int can_thin(the_image, i, j, value)
   int   i, j;
   short **the_image, value;
{
   int   a, b, c, d, count,
         no_neighbor, one=1, zero=0;
   short temp[3][3];

       /**************************************
       *
       *   Copy the center pixel and its
       *   neighbors to the temp array.
       *
       ***************************************/

   for(a=-1; a<2; a++)
      for(b=-1; b<2; b++)
         temp[a+1][b+1] = the_image[i+a][b+j];

       /**************************************
       *
       *   Set the center of temp to 0.
       *
       ***************************************/

   temp[1][1] = 0;

       /**************************************
       *
       *   Check the non-zero pixels in temp.
       *
       ***************************************/

   for(a=0; a<3; a++){
      for(b=0; b<3; b++){
         if(temp[a][b] == value){
            temp[a][b] = 0;

                /*********************************
                *
                *   Check the neighbors of this pixel
                *   If there is a single non-zero
                *   neighbor, set no_neighbor = 0.
                *
                **********************************/

            no_neighbor = 1;
            for(c=-1; c<2; c++){
               for(d=-1; d<2; d++){
                  if( ((a+c) >= 0)   &&
                      ((a+c) <= 2)   &&
                      ((b+d) >= 0)   &&
                      ((b+d) <= 2)){
                     if(temp[a+c][b+d] == value){
                        no_neighbor = 0;
                     }  /* ends if temp == value */
                  }  /* ends if part of temp array */
               }  /* ends loop over d */
            }  /* ends loop over c */
            temp[a][b] = value;

                /*********************************
                *
                *   If the non-zero pixel did not
                *   have any non-zero neighbors,
                *   no_neighbor still equals 1,
                *   and we cannot thin, so return
                *   zero.
                *
                **********************************/

            if(no_neighbor){
               return(zero);
            }
         }  /* ends if temp[a][b] == value */
      }  /* ends loop over b */
   }  /* ends loop over a */

       /**************************************
       *
       *   First, ensure the object is more
       *   than two wide.  If it is two wide,
       *   you will thin out the entire object.
       *   Check in all eight directions.
       *   If the distance to a zero is 0 or
       *   >= 2, then ok you can thin so go
       *   on to the remainder of this routine.
       *   If not, you cannot thin so return
       *   zero.
       *
       ***************************************/

   return(one);

}  /* ends can_thin */




     /*******************************************
     *
     *   special_closing(...
     *
     *   Closing is dilation followed by erosion.
     *   This routine will use the dilate_not_join
     *   dilation routine.  This will not allow
     *   two separate objects to join.
     *
     *   The number parameter specifies how
     *   dilations to perform before doing one
     *   erosion.
     *
     *******************************************/

int special_closing(the_image, out_image,
                value, threshold, number,
                rows, cols)
   int    number;
   short  **the_image,
          **out_image,
          threshold, value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;

   dilate_not_join(the_image, out_image,
                   value, threshold,
                   rows, cols);

   if(number > 1){
      count = 1;
      while(count < number){
         count++;
         dilate_not_join(the_image, out_image,
                         value, threshold,
                         rows, cols);
      }  /* ends while */
   }  /* ends if number > 1 */

   erosion(the_image, out_image,
           value, threshold,
           rows, cols);

return(1);
}  /* ends special_closing */




     /*******************************************
     *
     *   dilate_not_join(...
     *
     *   Use a variation of the grass fire
     *   wave front approach.
     *
     *   Raster scan the image left to right
     *   and examine and dilate the left edge pixels
     *   (a value to 0 transition).  Process them
     *   normally and "save" the result.  Next,
     *   raster scan the image right to left and
     *   save.  Raster scan top to bottom and save.
     *   Raster scan bottom to top and save.
     *
     *   That is one complete pass.
     *
     *******************************************/

int dilate_not_join(the_image, out_image,
                value, threshold,
                rows, cols)
   short  **the_image,
          **out_image,
          threshold, value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = the_image[i][j];

         /***************************
         *
         *   Scan left to right
         *   Look for value-0 transition
         *
         ****************************/

      printf("\n");
      for(i=1; i<rows-1; i++){
         if( (i%10) == 0) printf("%3d", i);
         for(j=1; j<cols-1; j++){
            if(the_image[i][j-1] == value  &&
               the_image[i][j]   == 0){
               count = 0;
               for(a=-1; a<=1; a++){
                   for(b=-1; b<=1; b++){
                         if(the_image[i+a][j+b]==value)
                            count++;
                   }  /*  ends loop over b */
               }  /* ends loop over a */
               if(count > threshold){
                  if(can_dilate(the_image,i,j,value)){
                     out_image[i][j] = value;
                  }  /* ends if can_dilate */
               }  /* ends if count > threshold */
            }  /* ends if the_image == value */
         }  /* ends loop over j */
      }  /* ends loop over i */

         /**************************************
         *
         *   Copy the output back to the input.
         *
         **************************************/

      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];


         /***************************
         *
         *   Scan right to left
         *   Do this by scanning left
         *   to right and look for
         *   0-value transition.
         *
         ****************************/

      printf("\n");
      for(i=1; i<rows-1; i++){
         if( (i%10) == 0) printf("%3d", i);
         for(j=1; j<cols-1; j++){
            if(the_image[i][j+1] == value   &&
               the_image[i][j]   == 0){
               count = 0;
               for(a=-1; a<=1; a++){
                   for(b=-1; b<=1; b++){
                         if(the_image[i+a][j+b]==value)
                            count++;
                   }  /*  ends loop over b */
               }  /* ends loop over a */
               if(count > threshold){
                  if(can_dilate(the_image,i,j,value)){
                     out_image[i][j] = value;
                  }  /* ends if can_dilate */
               }  /* ends if count > threshold */
            }  /* ends if the_image == value */
         }  /* ends loop over j */
      }  /* ends loop over i */

         /**************************************
         *
         *   Copy the output back to the input.
         *
         **************************************/

      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];


         /***************************
         *
         *   Scan top to bottom
         *   Look for value-0 transition
         *
         ****************************/

      printf("\n");
      for(j=1; j<cols-1; j++){
         if( (j%10) == 0) printf("%3d", j);
         for(i=1; i<rows-1; i++){
            if(the_image[i-1][j] == value   &&
               the_image[i][j]   == 0){
               count = 0;
               for(a=-1; a<=1; a++){
                   for(b=-1; b<=1; b++){
                         if(the_image[i+a][j+b]==value)
                            count++;
                   }  /*  ends loop over b */
               }  /* ends loop over a */
               if(count > threshold){
                  if(can_dilate(the_image,i,j,value)){
                     out_image[i][j] = value;
                  }  /* ends if can_dilate */
               }  /* ends if count > threshold */
            }  /* ends if the_image == value */
         }  /* ends loop over i */
      }  /* ends loop over j */

         /**************************************
         *
         *   Copy the output back to the input.
         *
         **************************************/

      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];



         /***************************
         *
         *   Scan bottom to top
         *   Do this by scanning top
         *   to bottom and look for
         *   0-value transition.
         *
         ****************************/

      printf("\n");
      for(j=1; j<cols-1; j++){
         if( (j%10) == 0) printf("%3d", j);
         for(i=1; i<rows-1; i++){
            if(the_image[i+1][j] == value   &&
               the_image[i][j]   == 0){
               count = 0;
               for(a=-1; a<=1; a++){
                   for(b=-1; b<=1; b++){
                         if(the_image[i+a][j+b]==value)
                            count++;
                   }  /*  ends loop over b */
               }  /* ends loop over a */
               if(count > threshold){
                  if(can_dilate(the_image,i,j,value)){
                     out_image[i][j] = value;
                  }  /* ends if can_dilate */
               }  /* ends if count > threshold */
            }  /* ends if the_image == value */
         }  /* ends loop over i */
      }  /* ends loop over j */

         /**************************************
         *
         *   Copy the output back to the input.
         *
         **************************************/

      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];


   /****
   fix_edges(out_image, 3, rows, cols);
   *****/
return(1);
}  /* ends dilate_not_join */





     /*******************************************
     *
     *   can_dilate(...
     *
     *   This function decides if you can dilate
     *   (set to value) a pixel without joining
     *   two separate objects in a 3x3 area.
     *
     *   First, you grow regions inside the 3x3
     *   area.  Next, check if the center pixel
     *   has neighbors with differing values.
     *   If it does, you cannot dilate it because
     *   that would join two separate objects.
     *
     *******************************************/

int can_dilate(the_image, i, j, value)
   int   i, j;
   short **the_image, value;
{
   int   a, b, c, d, count, found=0,
         no_neighbor,
         stack_pointer=-1,
         stack_empty=1,
         stack[12][2],
         pop_a, pop_b,
         one=1,
         zero=0;
   short first_value, label = 2, temp[3][3];

       /**************************************
       *
       *   Copy the center pixel and its
       *   neighbors to the temp array.
       *
       ***************************************/

   for(a=-1; a<2; a++)
      for(b=-1; b<2; b++)
         temp[a+1][b+1] = the_image[i+a][b+j];

       /**************************************
       *
       *   Grow objects inside the temp array.
       *
       ***************************************/

   for(a=0; a<3; a++){
      for(b=0; b<3; b++){
         stack_empty   = 1;
         stack_pointer = -1;
         if(temp[a][b] == value){
            little_label_and_check(temp, stack, label,
                                   &stack_empty,
                                   &stack_pointer,
                                   a, b, value);
            found = 1;
         }  /* ends if temp == value */

         while(stack_empty == 0){
            pop_a = stack[stack_pointer][0]; /* POP */
            pop_b = stack[stack_pointer][1]; /* POP */
            --stack_pointer;
            if(stack_pointer <= 0){
               stack_pointer = 0;
               stack_empty   = 1;
            }  /* ends if stack_pointer */
            little_label_and_check(temp, stack, label,
                                   &stack_empty,
                                   &stack_pointer,
                                   pop_a, pop_b, value);
         }  /* ends while stack_empty == 0 */

         if(found){
            found = 0;
            label++;
         }  /* ends if object_found */
      }  /* ends loop over b */
   }  /* ends loop over a */

       /**************************************
       *
       *   Look at the center pixel.  If it
       *   has two non-zero neigbors whose
       *   pixels are not the same, then
       *   you cannot dilate.
       *
       ***************************************/

   first_value = -1;
   for(a=0; a<3; a++){
      for(b=0; b<3; b++){
         if(temp[a][b]  != 0 &&
            first_value == -1){
            first_value = temp[a][b];
         }
         if(temp[a][b]  != 0  &&
            first_value != -1){
            if(temp[a][b] != first_value){
               return(zero);
            }
         }
      }  /* ends loop over b */
   }  /* ends loop over a */

   return(one);

}  /* ends can_dilate */





     /*******************************************
     *
     *   little_label_and_check(...
     *
     *   This function labels the objects in
     *   in a 3x3 area.
     *
     *******************************************/

int little_label_and_check(temp, stack, label, stack_empty,
                       stack_pointer, a, b, value)
   int   a, b, stack[12][2],
         *stack_empty, *stack_pointer;
   short temp[3][3], label, value;
{
   int c, d;

   temp[a][b] = label;
   for(c=a-1; c<=a+1; c++){
      for(d=b-1; d<=b+1; d++){
         if(c >= 0      &&
            c <= 2      &&
            d >= 0      &&
            d <= 2)
            if(temp[c][d] == value){  /* PUSH */
               *stack_pointer = *stack_pointer + 1;
               stack[*stack_pointer][0] = c;
               stack[*stack_pointer][1] = d;
               *stack_empty = 0;
            }  /* ends if temp == value */
      }  /* ends loop over d */
   }  /* ends loop over c */

return(1);
}  /* ends little_label_and_check */





     /*******************************************
     *
     *   edm(..
     *
     *   This function calculates the Euclidean
     *   distance measure for objects in an image.
     *   It calculates the distance from any
     *   pixel=value to the nearest zero pixel
     *
     *******************************************/


int edm(the_image, out_image,
    value, rows, cols)
   short  **the_image,
          **out_image,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = 0;

      /***************************
      *
      *   Loop over image array
      *
      ****************************/

   printf("\n");

   for(i=0; i<rows; i++){
      if( (i%10) == 0) printf("%3d", i);
      for(j=0; j<cols; j++){
         if(the_image[i][j] == value)
            out_image[i][j] = distance_8(the_image,
                                         i, j,
                                         value,
                                         rows, cols);
      }  /* ends loop over j */
   }  /* ends loop over i */

return(1);
}  /* ends edm */



     /*******************************************
     *
     *   distance_8(..
     *
     *   This function finds the distance from
     *   a pixel to the nearest zero pixel.
     *   It search in all eight directions.
     *
     *******************************************/

int distance_8(the_image, a, b, value, rows, cols)
   int   a, b;
   short **the_image, value;
   long  cols, rows;
{
   int i, j, measuring;
   short dist1  = 0,
         dist2  = 0,
         dist3  = 0,
         dist4  = 0,
         dist5  = 0,
         dist6  = 0,
         dist7  = 0,
         dist8  = 0,
         result = 0;

      /* straight up */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      i--;
      if(i >= 0){
         if(the_image[i][j] == value)
            dist1++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   result = dist1;

      /* straight down */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      i++;
      if(i <= rows-1){
         if(the_image[i][j] == value)
            dist2++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   if(dist2 <= result)
      result = dist2;

      /* straight left */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j--;
      if(j >= 0){
         if(the_image[i][j] == value)
            dist3++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   if(dist3 <= result)
      result = dist3;

      /* straight right */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j++;
      if(j <= cols-1){
         if(the_image[i][j] == value)
            dist4++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   if(dist4 <= result)
      result = dist4;

      /* left and up */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j--;
      i--;
      if(j >= 0 && i>=0){
         if(the_image[i][j] == value)
            dist5++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   dist5 = (dist5*14)/10;
   if(dist5 <= result)
      result = dist5;

      /* right and up */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j++;
      i--;
      if(j <=cols-1 && i>=0){
         if(the_image[i][j] == value)
            dist6++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   dist6 = (dist6*14)/10;
   if(dist6 <= result)
      result = dist6;

      /* right and down */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j++;
      i++;
      if(j <=cols-1 && i<=rows-1){
         if(the_image[i][j] == value)
            dist7++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   dist7 = (dist7*14)/10;
   if(dist7 <= result)
      result = dist7;

      /* left and down */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j--;
      i++;
      if(j >=0 && i<=rows-1){
         if(the_image[i][j] == value)
            dist8++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   dist8 = (dist8*14)/10;
   if(dist8 <= result)
      result = dist8;

   return(result);
}  /* ends distance_8 */





     /*******************************************
     *
     *   mat(..
     *
     *   This function finds the medial axis
     *   transform for objects in an image.
     *   The mat are those points that are
     *   minimally distant to more than one
     *   boundary point.
     *
     *******************************************/


int mat(the_image, out_image,
    value, rows, cols)
   short  **the_image,
          **out_image,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k,
          length, width;

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = 0;

      /***************************
      *
      *   Loop over image array
      *
      ****************************/

   printf("\n");

   for(i=0; i<rows; i++){
      if( (i%10) == 0) printf("%3d", i);
      for(j=0; j<cols; j++){
         if(the_image[i][j] == value)
            out_image[i][j] = mat_d(the_image,
                                    i, j, value,
                                    rows, cols);
      }  /* ends loop over j */
   }  /* ends loop over i */
return(1);
}  /* ends mat */



     /*******************************************
     *
     *   mat_d(..
     *
     *   This function helps find the medial
     *   axis transform.
     *
     *   This function measures the distances
     *   from the point to a zero pixel in all
     *   eight directions.  Look for the two
     *   shortest distances in the eight distances.
     *   If the two shortest distances are
     *   equal, then the point in question
     *   is minimally distant to more than one
     *   boundary point.  Therefore, it is
     *   on the medial axis so return a value.
     *   Otherwise, return zero.
     *
     *******************************************/

int mat_d(the_image, a, b, value, rows, cols)
   int   a, b;
   short **the_image, value;
   long  cols, rows;
{
   int i, j, measuring;
   short dist1  = 0,
         dist2  = 0,
         dist3  = 0,
         dist4  = 0,
         dist5  = 0,
         dist6  = 0,
         dist7  = 0,
         dist8  = 0,
         min1   = GRAY_LEVELS,
         min2   = GRAY_LEVELS,
         result = 0;

      /* straight up */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      i--;
      if(i >= 0){
         if(the_image[i][j] == value)
            dist1++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   result = dist1;
   min1   = dist1;

      /* straight down */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      i++;
      if(i <= rows-1){
         if(the_image[i][j] == value)
            dist2++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   if(dist2 <= result)
      result = dist2;
   if(dist2 < min1){
      min2 = min1;
      min1 = dist2;
   }
   else
      if(dist2 < min2)
         min2 = dist2;

      /* straight left */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j--;
      if(j >= 0){
         if(the_image[i][j] == value)
            dist3++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   if(dist3 <= result)
      result = dist3;
   if(dist3 < min1){
      min2 = min1;
      min1 = dist3;
   }
   else
      if(dist3 < min2)
         min2 = dist3;

      /* straight right */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j++;
      if(j <= cols-1){
         if(the_image[i][j] == value)
            dist4++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   if(dist4 <= result)
      result = dist4;
   if(dist4 < min1){
      min2 = min1;
      min1 = dist4;
   }
   else
      if(dist4 < min2)
         min2 = dist4;

      /* left and up */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j--;
      i--;
      if(j >= 0 && i>=0){
         if(the_image[i][j] == value)
            dist5++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   dist5 = ((dist5*14)+7)/10;
   if(dist5 <= result)
      result = dist5;
   if(dist5 < min1){
      min2 = min1;
      min1 = dist5;
   }
   else
      if(dist5 < min2)
         min2 = dist5;

      /* right and up */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j++;
      i--;
      if(j <=cols-1 && i>=0){
         if(the_image[i][j] == value)
            dist6++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   dist6 = ((dist6*14)+7)/10;
   if(dist6 <= result)
      result = dist6;
   if(dist6 < min1){
      min2 = min1;
      min1 = dist6;
   }
   else
      if(dist6 < min2)
         min2 = dist6;

      /* right and down */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j++;
      i++;
      if(j <=cols-1 && i<=rows-1){
         if(the_image[i][j] == value)
            dist7++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   dist7 = ((dist7*14)+7)/10;
   if(dist7 <= result)
      result = dist7;
   if(dist7 < min1){
      min2 = min1;
      min1 = dist7;
   }
   else
      if(dist7 < min2)
         min2 = dist7;

      /* left and down */
   measuring = 1;
   i = a;
   j = b;
   while(measuring){
      j--;
      i++;
      if(j >=0 && i<=rows-1){
         if(the_image[i][j] == value)
            dist8++;
         else
            measuring = 0;
      }
      else
         measuring = 0;
   }  /* ends while measuring */
   dist8 = ((dist8*14)+7)/10;
   if(dist8 <= result)
      result = dist8;
   if(dist8 < min1){
      min2 = min1;
      min1 = dist8;
   }
   else
      if(dist8 < min2)
         min2 = dist8;

   if(min1 == min2)
      result = value;
   else
      result = 0;

   if(min1 == 0)
      result = 0;

   return(result);
}  /* ends mat_d */
