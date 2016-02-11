

    /*********************************************
    *
    *       file xemboss.c
    *
    *       Functions: This file contains
    *          main
    *          emboss_convolution
    *          copy_array
    *
    *       Purpose:
    *          This program performs embossing on
    *          images using a collection of 
    *          convolution masks.
    *
    *       External Calls:
    *          imageio.c - create_image_file
    *                  read_image_array
    *                  write_image_array
    *                  get_image_size
    *                  allocate_image_array
    *                  free_image_array
    *
    *       Modifications:
    *          11 May 1999 - created
    *
    ***********************************************/

#include "cips.h"

   /* list the convolution masks */

short emboss_0_mask[3][3] =  {
       {-1,  0,  1},
       {-1,  1,  1},
       {-1,  0,  1} };

short emboss_1_mask[3][3] =  {
       { 0,  1,  1},
       {-1,  1,  1},
       {-1, -1,  0} };

short emboss_2_mask[3][3] =  {
       { 1,  1,  0},
       { 1,  1, -1},
       { 0, -1, -1} };

short emboss_3_mask[3][3] =  {
       { 1,  0, -1},
       { 1,  1, -1},
       { 1,  0, -1} };

short emboss_4_mask[3][3] =  {
       { 0, -1, -1},
       { 1,  1, -1},
       { 1,  1,  0} };

short emboss_5_mask[3][3] =  {
       {-1, -1, -1},
       { 0,  1,  0},
       { 1,  1,  1} };

short emboss_6_mask[3][3] =  {
       {-1, -1,  0},
       {-1,  1,  1},
       { 0,  1,  1} };




short emboss_7_mask[3][3] =  {
       {-1,  1,  1},
       {-1,  1,  1},
       {-1,  1,  1} };

short emboss_8_mask[3][3] =  {
       { 1,  1,  1},
       {-1,  1,  1},
       {-1, -1,  1} };

short emboss_9_mask[3][3] =  {
       { 1,  1,  1},
       { 1,  1, -1},
       { 1, -1, -1} };

short emboss_10_mask[3][3] =  {
       { 1,  1, -1},
       { 1,  1, -1},
       { 1,  1, -1} };

short emboss_11_mask[3][3] =  {
       { 1, -1, -1},
       { 1,  1, -1},
       { 1,  1,  1} };

short emboss_12_mask[3][3] =  {
       {-1, -1, -1},
       { 1,  1,  1},
       { 1,  1,  1} };

short emboss_13_mask[3][3] =  {
       {-1, -1,  1},
       {-1,  1,  1},
       { 1,  1,  1} };




short **the_image;
short **out_image;

main(argc, argv)
   int argc;
   char *argv[];
{

   char in_name[80], out_name[80];
   int  i, j, size, type;
   long bitsperpixel, length, width;

     /* Get the command line parameters */

   if(argc != 4){
    printf("\n"
    "\n   usage: xemboss in-file out-file"
    " type (0 thru 13)"
    "\n  ");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   strcpy(out_name, argv[2]);
   type      = atoi(argv[3]);

      /* Read the input file */

   if(does_not_exist(in_name)){
      printf("\nERROR input file %s does not exist",
             in_name);
      exit(0);
   }  /* ends if does_not_exist */

   create_image_file(in_name, out_name);
   get_image_size(in_name, &length, &width);
   get_bitsperpixel(in_name, &bitsperpixel);
   the_image = allocate_image_array(length, width);
   out_image = allocate_image_array(length, width);
   read_image_array(in_name, the_image);


   emboss_convolution(the_image, out_image,
               type,
               length, width,
               bitsperpixel);

   write_image_array(out_name, out_image);
   free_image_array(the_image, length);
   free_image_array(out_image, length);

}  /* ends main  */




   /**********************************************
   *
   *   emoboss_convolution(...
   *
   *   This function performs convolution 
   *   between the input image and a 3x3 mask.  
   *   The result is placed in the out_image.
   *
   ***********************************************/

emboss_convolution(image, out_image,
                   type,
                   rows, cols, 
                   bitsperpixel)
   short **image,
         **out_image;
   int   type;
   long  rows, cols, bitsperpixel;
{

   int a, b, i, is_present, j, sum;

   short  mask[3][3],
          max,
          min;

   min = 0;
   max = 255;
   if(bitsperpixel == 4)
      max = 16;

     /* clear output image array */
   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         out_image[i][j] = image[i][j];


   copy_array(type, mask);

   for(i=1; i<rows-1; i++){
      for(j=1; j<cols-1; j++){

         sum = 0;
         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               sum = sum + image[i+a][j+b] *
                     mask[a+1][b+1];
            }
         }

         if(sum > max) sum = max;
         if(sum < 0)   sum = 0;
         out_image[i][j] = sum;
         if( type >= 7   &&
             type <= 13)
            out_image[i][j] = out_image[i][j]/3;

      }  /* ends loop over j */
   }  /* ends loop over i */

}  /* ends emboss_convolution */




copy_array(type, mask)
   int type;
   short mask[3][3];
{
   int i, j;

   for(i=0; i<3; i++){
      for(j=0; j<3; j++){
         switch(type){

            case 0:
            mask[i][j] = emboss_0_mask[i][j];
            break;

            case 1:
            mask[i][j] = emboss_1_mask[i][j];
            break;

            case 2:
            mask[i][j] = emboss_2_mask[i][j];
            break;

            case 3:
            mask[i][j] = emboss_3_mask[i][j];
            break;

            case 4:
            mask[i][j] = emboss_4_mask[i][j];
            break;

            case 5:
            mask[i][j] = emboss_5_mask[i][j];
            break;

            case 6:
            mask[i][j] = emboss_6_mask[i][j];
            break;

            case 7:
            mask[i][j] = emboss_7_mask[i][j];
            break;

            case 8:
            mask[i][j] = emboss_8_mask[i][j];
            break;

            case 9:
            mask[i][j] = emboss_9_mask[i][j];
            break;

            case 10:
            mask[i][j] = emboss_10_mask[i][j];
            break;

            case 11:
            mask[i][j] = emboss_11_mask[i][j];
            break;

            case 12:
            mask[i][j] = emboss_12_mask[i][j];
            break;

            case 13:
            mask[i][j] = emboss_13_mask[i][j];
            break;

         }  /* ends switch */
      }  /* ends loop over j */
   }  /* ends loop over i */

}  /* ends copy_array */
