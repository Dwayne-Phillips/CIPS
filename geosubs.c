
    /********************************************
    *
    *    file geosubs.c
    *
    *    Functions: This file contains
    *       geometry
    *       arotate
    *       bilinear_interpolate
    *
    *    Purpose:
    *       These functions performs different
    *       geometric operations.
    *
    *    External Calls:
    *       none
    *
    *    Modifications:
    *       20 October 1993- created
    *       27 August 1998 - modified to work on 
    *            entire images at once.
    *
    ********************************************/

#include "cips.h"

#define FILL 150





     /*******************************************
     *
     *   geometry(..
     *
     *   This routine performs geometric
     *   transformations on the pixels in an
     *   image array.  It performs basic
     *   displacement, stretching, and rotation.
     *
     *   The basic equations are:
     *
     *   new x = x.cos(a) + y.sin(a) + x_displace
     *           + x.x_stretch +x.y.x_cross
     *
     *   new y = y.cos(a) - x.sin(a) + y_displace
     *           + y.y_stretch +x.y.y_cross
     *
     *******************************************/

geometry(the_image, out_image,
        x_angle,
        x_stretch, y_stretch,
        x_displace, y_displace,
        x_cross, y_cross,
        bilinear,
        rows,
        cols)

   float  x_angle, x_stretch, y_stretch,
          x_cross, y_cross;
   int    bilinear;
   long   cols, rows;
   short  **the_image,
          **out_image,
          x_displace, y_displace;
{
   double cosa, sina, radian_angle, tmpx, tmpy;
   float  fi, fj, x_div, y_div, x_num, y_num;
   int    i, j, new_i, new_j;


      /******************************
      *
      *   Load the terms array with
      *   the correct parameters.
      *
      *******************************/

      /* the following magic number is from
         180 degrees divided by pi */
   radian_angle = x_angle/57.29577951;
   cosa  = cos(radian_angle);
   sina  = sin(radian_angle);

      /************************************
      *
      *   NOTE: You divide by the
      *   stretching factors. Therefore, if
      *   they are zero, you divide by 1.
      *   You do this with the x_div y_div
      *   variables. You also need a
      *   numerator term to create a zero
      *   product.  You do this with the
      *   x_num and y_num variables.
      *
      *************************************/

   if(x_stretch < 0.00001){
      x_div = 1.0;
      x_num = 0.0;
   }
   else{
      x_div = x_stretch;
      x_num = 1.0;
   }

   if(y_stretch < 0.00001){
      y_div = 1.0;
      y_num = 0.0;
   }
   else{
      y_div = y_stretch;
      y_num = 1.0;
   }

      /**************************
      *
      *   Loop over image array
      *
      **************************/

   printf("\n");
   for(i=0; i<rows; i++){
      if( (i%10) == 0) printf("%d ", i);
      for(j=0; j<cols; j++){

         fi = i;
         fj = j;

         tmpx = (double)(j)*cosa         +
                (double)(i)*sina         +
                (double)(x_displace)     +
                (double)(x_num*fj/x_div) +
                (double)(x_cross*i*j);

         tmpy = (double)(i)*cosa         -
                (double)(j)*sina         +
                (double)(y_displace)     +
                (double)(y_num*fi/y_div) +
                (double)(y_cross*i*j);

         if(x_stretch != 0.0)
            tmpx = tmpx - (double)(fj*cosa + fi*sina);
         if(y_stretch != 0.0)
            tmpy = tmpy - (double)(fi*cosa - fj*sina);

         new_j = tmpx;
         new_i = tmpy;

         if(bilinear == 0){
            if(new_j < 0       ||
               new_j >= cols   ||
               new_i < 0       ||
               new_i >= rows)
               out_image[i][j] = FILL;
            else
               out_image[i][j] =
                the_image[new_i][new_j];
         }  /* ends if bilinear */
         else{
            out_image[i][j] = 
               bilinear_interpolate(the_image,
                                    tmpx, tmpy,
                                    rows, cols);
         }  /* ends bilinear if */

      }  /* ends loop over j */
   }  /* ends loop over i */

}  /* ends geometry */




     /*******************************************
     *
     *   arotate(..
     *
     *   This routine performs rotation about
     *   any point m,n.
     *
     *   The basic equations are:
     *
     *   new x = x.cos(a) - y.sin(a)
     *           -m.cos(a) + m + n.sin(a)
     *
     *   new y = y.cos(a) + x.sin(a)
     *           -m.sin(a) - n.cos(a) + n
     *
     *******************************************/

arotate(the_image, out_image,
        angle,
        m, n, bilinear,
        rows, cols)
   float  angle;
   int    bilinear;
   long   cols, rows;
   short  **the_image,
          **out_image,
          m, n;
{
   double cosa, sina, radian_angle, tmpx, tmpy;
   int    i, j, new_i, new_j;


      /* the following magic number is from
         180 degrees divided by pi */
   radian_angle = angle/57.29577951;
   cosa  = cos(radian_angle);
   sina  = sin(radian_angle);

      /**************************
      *
      *   Loop over image array
      *
      **************************/

   printf("\n");
   for(i=0; i<rows; i++){
      if( (i%10) == 0) printf("%d ", i);
      for(j=0; j<cols; j++){

     /******************************************
     *
     *   new x = x.cos(a) - y.sin(a)
     *           -m.cos(a) + m + n.sin(a)
     *
     *   new y = y.cos(a) + x.sin(a)
     *           -m.sin(a) - n.cos(a) + n
     *
     *******************************************/

         tmpx = (double)(j)*cosa    -
                (double)(i)*sina    -
                (double)(m)*cosa    +
                (double)(m)         +
                (double)(n)*sina;

         tmpy = (double)(i)*cosa    +
                (double)(j)*sina    -
                (double)(m)*sina    -
                (double)(n)*cosa    +
                (double)(n);

         new_j = tmpx;
         new_i = tmpy;

         if(bilinear == 0){
            if(new_j < 0       ||
               new_j >= cols   ||
               new_i < 0       ||
               new_i >= rows)
               out_image[i][j] = FILL;
            else
               out_image[i][j] =
                the_image[new_i][new_j];
         }  /* ends if bilinear */
         else{
            out_image[i][j] = 
               bilinear_interpolate(the_image,
                                    tmpx, tmpy,
                                    rows, cols);
         }  /* ends bilinear if */

      }  /* ends loop over j */
   }  /* ends loop over i */

}  /* ends arotate */





     /*******************************************
     *
     *   bilinear_interpolate(..
     *
     *   This routine performs bi-linear
     *   interpolation.
     *
     *   If x or y is out of range, i.e. less
     *   than zero or greater than rows or cols,
     *   this routine returns a zero.
     *
     *   If x and y are both in range, this
     *   routine interpolates in the horizontal
     *   and vertical directions and returns
     *   the proper gray level.
     *
     *******************************************/

bilinear_interpolate(the_image, x, y, rows, cols)
   double x, y;
   long   cols, rows;
   short  **the_image;
{
   double fraction_x, fraction_y,
          one_minus_x, one_minus_y,
          tmp_double;
   int    ceil_x, ceil_y, floor_x, floor_y;
   short  p1, p2, p3, result = FILL;

      /******************************
      *
      *   If x or y is out of range,
      *   return a FILL.
      *
      *******************************/

   if(x < 0.0               ||
      x >= (double)(cols-1)   ||
      y < 0.0               ||
      y >= (double)(rows-1))
      return(result);

   tmp_double = floor(x);
   floor_x    = tmp_double;
   tmp_double = floor(y);
   floor_y    = tmp_double;
   tmp_double = ceil(x);
   ceil_x     = tmp_double;
   tmp_double = ceil(y);
   ceil_y     = tmp_double;

   fraction_x = x - floor(x);
   fraction_y = y - floor(y);

   one_minus_x = 1.0 - fraction_x;
   one_minus_y = 1.0 - fraction_y;

   tmp_double = one_minus_x * 
          (double)(the_image[floor_y][floor_x]) +
          fraction_x * 
          (double)(the_image[floor_y][ceil_x]);
   p1         = tmp_double;

   tmp_double = one_minus_x * 
          (double)(the_image[ceil_y][floor_x]) +
          fraction_x * 
          (double)(the_image[ceil_y][ceil_x]);
   p2         = tmp_double;

   tmp_double = one_minus_y * (double)(p1) +
          fraction_y * (double)(p2);
   p3         = tmp_double;


   return(p3);

}  /* ends bilinear_interpolate */



#ifdef NEVER

     /*******************************************
     *
     *   get_geometry_options(..
     *
     *   This routine interacts with the user
     *   to obtain the parameters to call the
     *   geometry operations subroutines.
     *
     *******************************************/

get_geometry_options(operation, angle, 
                     x_displace, y_displace, 
                     x_stretch, y_stretch, 
                     x_cross,  y_cross, 
                     bilinear, m, n)
   char  operation[];
   int   *bilinear;
   short *m, *n, *x_displace, *y_displace;
   float *angle,
         *x_cross, *y_cross, 
         *x_stretch, *y_stretch;
{
   int not_finished, response;
   not_finished = 1;
   while(not_finished){

      printf("\nThe geomety options are:");
      printf("\n\t1. Operation is %s", operation);
      printf("\n\t2. Angle is %f", *angle);
      printf("\n\t3. x-displace=%d y-displace=%d",
         *x_displace, *y_displace);
      printf("\n\t4. x-stretch=%f y-stretch=%f",
         *x_stretch, *y_stretch);
      printf("\n\t5. x-cross=%f y-cross=%f",
         *x_cross, *y_cross);
      printf("\n\t6. bilinear = %d", *bilinear);
      printf("\n\t7. rotation points m=%d n=%d",
         *m, *n);
      printf("\n\nExamples:");
      printf("\ngeometry needs: angle");
      printf(" x-displace y-displace");
      printf(" x-stretch y-stretch");
      printf("\n                x-cross y-cross");
      printf(" bilinear (1 or 0)");
      printf("\nrotate needs: angle m n");
      printf(" bilinear (1 or 0)");
      printf("\n\nEnter choice (0 = no change) _\b");
      get_integer(&response);

      if(response == 0)
         not_finished = 0;

      if(response == 1){
         printf("\nEnter operation:");
         gets(operation);
      }  /* ends if 1 */

      if(response == 2){
         printf("\nEnter angle: ___\b\b\b");
         get_float(angle);
      }  /* ends if 2 */

      if(response == 3){
         printf("\nEnter x-displace: ___\b\b\b");
         get_integer(x_displace);
         printf("\nEnter y-displace: ___\b\b\b");
         get_integer(y_displace);
      }  /* ends if 3 */

      if(response == 4){
         printf("\nEnter x-stretch: ___\b\b\b");
         get_float(x_stretch);
         printf("\nEnter y-stretch: ___\b\b\b");
         get_float(y_stretch);
      }  /* ends if 4 */

      if(response == 5){
         printf("\nEnter x-cross: ___\b\b\b");
         get_float(x_cross);
         printf("\nEnter y-cross: ___\b\b\b");
         get_float(y_cross);
      }  /* ends if 5 */

      if(response == 6){
         printf("\nEnter bilinear: _\b");
         get_integer(bilinear);
      }  /* ends if 6 */

      if(response == 7){
         printf("\nEnter rotation point m: _\b");
         get_integer(m);
         printf("\nEnter rotation point n: _\b");
         get_integer(n);
      }  /* ends if 7 */


   }  /* ends while not_finished */

}  /* ends get_geometry_options */

#endif
