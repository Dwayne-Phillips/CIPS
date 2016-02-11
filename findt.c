
    /***********************************************
    *
    *  file d:\cips\findt.c
    *
    *  Functions: This file contains
    *     main
    *     find_text
    *
    *  Purpose:
    *     This file contains the main calling
    *     routine and subroutines to find
    *     text on hidden top of an image.  
    *
    *  External Calls:
    *     tiff.c - read_tiff_header
    *     rtiff.c - read_tiff_image
    *     wtiff.c - write_array_into_tiff_image
    *
    *  Modifications:
    *     16 February 1998 - created
    *
    *************************************************/

#include "cips.h"


short the_image[ROWS][COLS];
short out_image[ROWS][COLS];

main(argc, argv)
   int argc;
   char *argv[];
{

   char  name[80], name2[80];
   char  response[80];
   float factor;
   int   i, ie, il,
         j, le, length, ll, width;



   struct   tiff_header_struct image_header;

   if(argc < 3){
    printf("\n\nNot enough parameters:");
    printf("\n");
    printf("\n   usage: hidet image-file text-file ");
    exit(0);
   }

   strcpy(name, argv[1]);
   strcpy(name2, argv[2]);

   il = 1;
   ie = 1;
   ll = ROWS+1;
   le = COLS+1;

   read_tiff_header(name, &image_header);
   /***create_file_if_needed(name, name2, out_image);***/

   length = (ROWS-10 + image_header.image_length)/ROWS;
   width  = (COLS-10 +image_header.image_width)/COLS;
   printf("\nlength=%d  width=%d", length, width);
printf("\nHit enter to continue");
gets(response);

   for(i=0; i<length; i++){
      for(j=0; j<width; j++){

         find_text(name, name2,
            the_image, out_image,
            il+i*ROWS, ie+j*COLS, 
            il+i*ROWS+ROWS, ie+j*COLS+COLS);

      } /* ends loop over j */
   }  /* ends loop over i */
}  /* ends main  */






   /**************************************************
   *
   *   find_text(...
   *
   *   If a pixel in in2 is non-zero, then the same
   *   pixel in in1 is set to itself plus a percent
   *   factor.
   *
   ***************************************************/

find_text(factor, in1_name, in2_name, 
          the_image, out_image,
          il, ie, ll, le)
   char   in1_name[], in2_name[];
   int    il, ie, ll, le;
   float  factor;
   short  the_image[ROWS][COLS],
          out_image[ROWS][COLS];
{
   int    a, b, i, j, length, width;
   int    diff;
   float  fdiff, fpixel;
   struct tiff_header_struct image_header;

   for(i=0; i<ROWS; i++)
      for(j=0; j<COLS; j++)
         out_image[i][j] = 0;

   read_tiff_image(in1_name, the_image,
                   il, ie, ll, le);

   for(i=1; i<ROWS-1; i++){
      for(j=1; j<COLS-1; j++){

         for(a=-1; a<2; a++){
            for(b=-1; b<2; b++){
               fdiff = (float)(the_image[i][j] - the_image[i+a][j+b]);
               fpixel = (float)(the_image[i][j]);
               if(fdiff >= (fpixel*0.1))
               out_image[i][j] = 200;
            }
         }

      }  /* ends loop over j */
   }  /* ends loop over i */

   write_array_into_tiff_image(in2_name, out_image,
                               il, ie, ll, le);

} /* ends find_text */
