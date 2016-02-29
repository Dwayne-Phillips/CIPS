

    /********************************************
    *
    *   file stretch.c
    *
    *   Functions: This file contains
    *      main
    *      stretch
    *      bilinear_interpolate
    *
    *   Purpose:
    *      This file contains the main calling
    *      routine and the needed subroutines
    *      for a program which stretches
    *      an image by any factor.  It can either
    *      roundoff the numbers or use
    *      bi-linear interpolation.
    *
    *   External Calls:
    *      imageio.c - create_resized_image_file
    *                  read_image_array
    *                  write_image_array
    *                  get_image_size
    *                  allocate_image_array
    *                  free_image_array
    *
    *   Modifications:
    *      4 December 1993 - created
    *     16 September 1998 - modified to work on entire
    *         images at one time.
    *     22 September 1998 - modified to work with 
    *           all I O routines in imageio.c.
    *
    *********************************************/

#include "cips.h"
#define FILL 150


short **the_image;
short **out_image;

main(argc, argv)
   int argc;
   char *argv[];
{

   char     in_name[80], out_name[80];

   float    x_stretch, y_stretch;

   int      bilinear; 
   long     tmp_length, tmp_width;
   long     length, width;
   
   struct bmpfileheader      bmp_file_header;
   struct bitmapheader       bmheader;
   struct bitmapheader       bmheader2;
   struct tiff_header_struct tiff_file_header;
   struct tiff_header_struct tiff_file_header2;

       /******************************************
       *
       *  Interpret the command line parameters.
       *
       *******************************************/

   if(argc < 6 || argc > 6){
    printf(
     "\n"
     "\n usage: stretch in-file out-file x-stretch "
     "y-stretch bilinear (1 or 0)"
     "\n");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   strcpy(out_name, argv[2]);
   x_stretch = atof(argv[3]);
   y_stretch = atof(argv[4]);
   bilinear  = atoi(argv[5]);

   if(does_not_exist(in_name)){
    printf("\nERROR input file %s does not exist",
             in_name);
    exit(0);
   }

       /********************************************
       *
       *   Create an output file different in size
       *   from the input file.
       *
       ********************************************/

   get_image_size(in_name, &length, &width);
   tmp_length = ((float)(length)*y_stretch);
   tmp_width  = ((float)(width)*x_stretch);
   create_resized_image_file(in_name, out_name,
                             tmp_length, tmp_width);

   the_image = allocate_image_array(length, width);
   out_image = allocate_image_array(tmp_length, tmp_width);

   read_image_array(in_name, the_image);

   stretch(the_image, out_image,
           x_stretch, y_stretch,
           bilinear,
           tmp_length,
           tmp_width,
           length,
           width);

   write_image_array(out_name, out_image);
   free_image_array(out_image, tmp_length);
   free_image_array(the_image, length);

}  /* ends main  */




     /*******************************************
     *
     *   stretch(..
     *
     *   This routine performs the image 
     *   stretching. If bilinear == 0, it uses 
     *   the roundoff approach for enlarging 
     *   an area. If bilinear == 1, it calls the 
     *   bilinear_interpolate routine to get 
     *   the value of a pixel that lies 
     *   between pixels.
     *
     ********************************************/

stretch(the_image, out_image,
        x_stretch, y_stretch,
        bilinear,
        out_rows, out_cols,
        in_rows,  in_cols)

   float  x_stretch, y_stretch;
   int    bilinear;
   short  **the_image,
          **out_image;
   long   out_cols, out_rows;
   long   in_cols,  in_rows;
{
   double tmpx, tmpy;
   float  fi, fj;
   int    i, j, new_i, new_j;

      /**************************
      *
      *   Loop over image array
      *
      **************************/

   printf("\n");
   for(i=0; i<out_rows; i++){
      if( (i%10) == 0) printf("%d ", i);
      for(j=0; j<out_cols; j++){

         fi = i;
         fj = j;

         tmpx = fj/x_stretch;
         tmpy = fi/y_stretch;

         new_i = tmpy;
         new_j = tmpx;

         if(bilinear == 0){
            if(new_j < 0       ||
               new_j >= in_cols   ||
               new_i < 0       ||
               new_i >= in_rows)
               out_image[i][j] = FILL;
            else
               out_image[i][j] =
                the_image[new_i][new_j];
         }  /* ends if bilinear */
         else{
            out_image[i][j] =
               bilinear_interpolate(the_image,
                                    tmpx, tmpy,
                                    in_rows, in_cols);
         }  /* ends bilinear if */

      }  /* ends loop over j */
   }  /* ends loop over i */

}  /* ends stretch */

