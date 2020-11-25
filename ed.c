
       /**********************************************
       *
       *   file ed.c
       *
       *   Functions: This file contains
       *      erosion
       *      dilation
       *      mask_erosion
       *      mask_dilation
       *      interior_outline
       *      exterior_outline
       *      copy_3_x_3
       *      opening
       *      closing
       *      get_shape_options
       *
       *   Purpose:
       *      These functions perform the erosion,
       *      dilation, outlining, opening and
       *      closing operations.
       *
       *   External Calls:
       *      none
       *
       *   Modifications:
       *      14 March 1993 - created
       *      21 August 1998 - modified to work on entire
       *           images at once.
       *
       ************************************************/

#include "cips.h"

int copy_3_x_3();


short edmask1[3][3] = {{0, 1, 0},
                       {0, 1, 0},
                       {0, 1, 0}};

short edmask2[3][3] = {{0, 0, 0},
                       {1, 1, 1},
                       {0, 0, 0}};

short edmask3[3][3] = {{0, 1, 0},
                       {1, 1, 1},
                       {0, 1, 0}};

short edmask4[3][3] = {{1, 1, 1},
                       {1, 1, 1},
                       {1, 1, 1}};






     /*******************************************
     *
     *   erosion(...
     *
     *   This function performs the erosion
     *   operation.  If a value pixel has more
     *   than the threshold number of 0
     *   neighbors, you erode it by setting it
     *   to 0.
     *
     *******************************************/

int erosion(the_image, out_image,
        value, threshold,
        rows, cols)

   int    threshold;
   short  **the_image,
          **out_image,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;

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
                   if( (i+a) >= 0){
                      if(the_image[i+a][j+b] == 0)
                         count++;
                   }
                }  /*  ends loop over b */
            }  /* ends loop over a */
            if(count > threshold){ out_image[i][j] = 0;
            }
         }  /* ends if the_image == value */
      }  /* ends loop over j */
   }  /* ends loop over i */

   /*****
   fix_edges(out_image, 3, rows, cols);
   ***/
return(1);
}  /* ends erosion */





     /*******************************************
     *
     *   dilation(...
     *
     *   This function performs the dilation
     *   operation.  If a 0 pixel has more than
     *   threshold number of value neighbors,
     *   you dilate it by setting it to value.
     *
     *******************************************/

int dilation(the_image, out_image,
         value, threshold,
         rows, cols)
   int    threshold;
   short  **the_image,
          **out_image,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;
   int    three = 3;

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
         out_image[i][j] = the_image[i][j];
         if(the_image[i][j] == 0){
            count = 0;
            for(a=-1; a<=1; a++){
                for(b=-1; b<=1; b++){
                   if(a!=0  &&  b!=0){
                      if(the_image[i+a][j+b] == value)
                         count++;
                   }  /* ends avoid the center pixel */
                }  /*  ends loop over b */
            }  /* ends loop over a */
            if(count > threshold)
               out_image[i][j] = value;
         }  /* ends if the_image == 0 */
      }  /* ends loop over j */
   }  /* ends loop over i */

   /*****
   fix_edges(out_image, three, rows, cols);
   ***/

return(1);
}  /* ends dilation */




     /*******************************************
     *
     *   mask_dilation(...
     *
     *   This function performs the dilation
     *   operation using the erosion-dilation
     *   3x3 masks given above.  It works on
     *   0-value images.
     *
     *******************************************/

int mask_dilation(the_image, out_image,
              value, mask_type,
              rows, cols)
   int    mask_type;
   short  **the_image,
          **out_image,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;
   short  mask[3][3], max;

      /**************************************
      *
      *   Copy the 3x3 erosion-dilation mask
      *   specified by the mask_type.
      *
      ***************************************/

   switch(mask_type){
      case 1:
         copy_3_x_3(mask, edmask1);
         break;
      case 2:
         copy_3_x_3(mask, edmask2);
         break;
      case 3:
         copy_3_x_3(mask, edmask3);
         break;
      case 4:
         copy_3_x_3(mask, edmask4);
         break;
      default:
         printf("\nInvalid mask type, using mask 4");
         copy_3_x_3(mask, edmask4);
         break;
   }

      /***************************
      *
      *   Loop over image array
      *
      ****************************/


   printf("\n");

   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%3d", i);
      for(j=1; j<cols-1; j++){
         max = 0;
         for(a=-1; a<=1; a++){
             for(b=-1; b<=1; b++){
                if(mask[a+1][b+1] == 1){
                   if(the_image[i+a][j+b] > max)
                      max = the_image[i+a][j+b];
                }  /* ends if mask == 1 */
             }  /*  ends loop over b */
         }  /* ends loop over a */
         out_image[i][j] = max;
      }  /* ends loop over j */
   }  /* ends loop over i */

   /*****
   fix_edges(out_image, 3, rows, cols);
   ***/
return(1);
}  /* ends mask_dilation */





     /*******************************************
     *
     *   mask_erosion(...
     *
     *   This function performs the erosion
     *   operation using the erosion-dilation
     *   3x3 masks given above.  It works on
     *   0-value images.
     *
     *******************************************/

int mask_erosion(the_image, out_image,
             value, mask_type,
             rows, cols)
   int    mask_type;
   short  **the_image,
          **out_image,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;
   short  mask[3][3], min;

      /**************************************
      *
      *   Copy the 3x3 erosion-dilation mask
      *   specified by the mask_type.
      *
      ***************************************/

   switch(mask_type){
      case 1:
         copy_3_x_3(mask, edmask1);
         break;
      case 2:
         copy_3_x_3(mask, edmask2);
         break;
      case 3:
         copy_3_x_3(mask, edmask3);
         break;
      case 4:
         copy_3_x_3(mask, edmask4);
         break;
      default:
         printf("\nInvalid mask type, using mask 4");
         copy_3_x_3(mask, edmask4);
         break;
   }


      /***************************
      *
      *   Loop over image array
      *
      ****************************/

   printf("\n");

   for(i=1; i<rows-1; i++){
      if( (i%10) == 0) printf("%3d", i);
      for(j=1; j<cols-1; j++){
         min = value;
         for(a=-1; a<=1; a++){
             for(b=-1; b<=1; b++){
                if(mask[a+1][b+1] == 1){
                   if(the_image[i+a][j+b] < min)
                      min = the_image[i+a][j+b];
                }  /* ends if mask == 1 */
             }  /*  ends loop over b */
         }  /* ends loop over a */
         out_image[i][j] = min;
      }  /* ends loop over j */
   }  /* ends loop over i */

   /*****
   fix_edges(out_image, 3, rows, cols);
   ***/
return(1);
}  /* ends mask_erosion */



     /***********************************************
     *
     *   copy_3_x_3(a, b)
     *
     *   This function copies a 3x3 array of shorts
     *   from one array to another.  It copies array
     *   b into array a.
     *
     ***********************************************/

int copy_3_x_3(a, b)
   short a[3][3], b[3][3];
{
   int i, j;
   for(i=0; i<3; i++)
      for(j=0; j<3; j++)
         a[i][j] = b[i][j];
return(1);
}  /* ends copy_3_x_3 */




     /*******************************************
     *
     *   opening(...
     *
     *   Opening is erosion followed by dilation.
     *   This routine will use the mask erosion
     *   and dilation.  You could use the other
     *   types and you could mix the two types.
     *
     *   The number parameter specifies how
     *   erosions to perform before doing one
     *   dilation.
     *
     *******************************************/

int opening(the_image, out_image,
        value, mask_type, number,
        rows, cols)
   int    number;
   int    mask_type;
   short  **the_image,
          **out_image,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;
   short  mask[3][3], max;

      /**************************************
      *
      *   Copy the 3x3 erosion-dilation mask
      *   specified by the mask_type.
      *
      ***************************************/

   switch(mask_type){
      case 1:
         copy_3_x_3(mask, edmask1);
         break;
      case 2:
         copy_3_x_3(mask, edmask2);
         break;
      case 3:
         copy_3_x_3(mask, edmask3);
         break;
      case 4:
         copy_3_x_3(mask, edmask4);
         break;
      default:
         printf("\nInvalid mask type, using mask 4");
         copy_3_x_3(mask, edmask4);
         break;
   }

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = the_image[i][j];

   mask_erosion(the_image, out_image,
                value, mask_type,
                rows, cols);

   if(number > 1){
      count = 1;
      while(count < number){
         count++;
         mask_erosion(the_image, out_image,
                      value, mask_type,
                      rows, cols);
      }  /* ends while */
   }  /* ends if number > 1 */

   mask_dilation(the_image,
                 out_image,
                 value, mask_type,
                 rows, cols);

return(1);
}  /* ends opening */





     /*******************************************
     *
     *   closing(...
     *
     *   Closing is dilation followed by erosion.
     *   This routine will use the mask erosion
     *   and dilation.  You could use the other
     *   types and you could mix the two types.
     *
     *   The number parameter specifies how
     *   dilations to perform before doing one
     *   erosion.
     *
     *******************************************/

int closing(the_image, out_image,
        value, mask_type, number,
        rows, cols)
   int    number;
   int    mask_type;
   short  **the_image,
          **out_image,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;
   short  mask[3][3], max;
printf("\nCLOSING> value=%d mask=%d number=%d",value,mask_type,number);

      /**************************************
      *
      *   Copy the 3x3 erosion-dilation mask
      *   specified by the mask_type.
      *
      ***************************************/

   switch(mask_type){
      case 1:
         copy_3_x_3(mask, edmask1);
         break;
      case 2:
         copy_3_x_3(mask, edmask2);
         break;
      case 3:
         copy_3_x_3(mask, edmask3);
         break;
      case 4:
         copy_3_x_3(mask, edmask4);
         break;
      default:
         printf("\nInvalid mask type, using mask 4");
         copy_3_x_3(mask, edmask4);
         break;
   }

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = the_image[i][j];

   mask_dilation(the_image, out_image,
                 value, mask_type,
                 rows, cols);

   if(number > 1){
      count = 1;
      while(count < number){
         count++;
         mask_dilation(the_image, out_image,
                        value, mask_type,
                        rows, cols);
      }  /* ends while */
   }  /* ends if number > 1 */

   mask_erosion(the_image, out_image,
                value, mask_type,
                rows, cols);

return(1);
}  /* ends closing */




     /*******************************************
     *
     *   interior_outline(...
     *
     *   This function produces the outline of
     *   any "holes" inside an object.  The
     *   method is:
     *      output = erosion of input
     *      final output = input - output
     *
     *******************************************/

int interior_outline(the_image, out_image,
                 value, mask_type,
                 rows, cols)
   int    mask_type;
   short  **the_image,
          **out_image,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;
   short  mask[3][3], max;

      /**************************************
      *
      *   Copy the 3x3 erosion-dilation mask
      *   specified by the mask_type.
      *
      ***************************************/

   switch(mask_type){
      case 1:
         copy_3_x_3(mask, edmask1);
         break;
      case 2:
         copy_3_x_3(mask, edmask2);
         break;
      case 3:
         copy_3_x_3(mask, edmask3);
         break;
      case 4:
         copy_3_x_3(mask, edmask4);
         break;
      default:
         printf("\nInvalid mask type, using mask 4");
         copy_3_x_3(mask, edmask4);
         break;
   }

   mask_erosion(the_image,
                out_image,
                value, mask_type,
                rows, cols);

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         the_image[i][j] =
            the_image[i][j] - out_image[i][j];

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = the_image[i][j];

return(1);
}  /* ends interior_outline */





     /*******************************************
     *
     *   exterior_outline(...
     *
     *   This function produces the outline of
     *   exterior of an object.  The
     *   method is:
     *      output = dilation of input
     *      final output = output - input
     *
     *******************************************/


int exterior_outline(the_image, out_image,
                 value, mask_type,
                 rows, cols)
   int    mask_type;
   short  **the_image,
          **out_image,
          value;
   long   cols, rows;
{
   int    a, b, count, i, j, k;
   short  mask[3][3], max;

      /**************************************
      *
      *   Copy the 3x3 erosion-dilation mask
      *   specified by the mask_type.
      *
      ***************************************/

   switch(mask_type){
      case 1:
         copy_3_x_3(mask, edmask1);
         break;
      case 2:
         copy_3_x_3(mask, edmask2);
         break;
      case 3:
         copy_3_x_3(mask, edmask3);
         break;
      case 4:
         copy_3_x_3(mask, edmask4);
         break;
      default:
         printf("\nInvalid mask type, using mask 4");
         copy_3_x_3(mask, edmask4);
         break;
   }

   mask_dilation(the_image, out_image,
                 value, mask_type,
                 rows, cols);

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         the_image[i][j] =
            out_image[i][j] - the_image[i][j];

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = the_image[i][j];

return(1);
}  /* ends exterior_outline */
