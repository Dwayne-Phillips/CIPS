
    /***********************************************
    *
    *  file stega.c
    *
    *  Functions: This file contains
    *     main
    *     hide_image
    *     hide_pixels
    *     uncover_image
    *     uncover_pixels
    *
    *  Purpose:
    *     This file contains the main calling
    *     routine and other routines that
    *     use steganography to hide one image
    *     inside another and then recover the
    *     hidden image.
    *
    *  External Calls:
    *      imageio.c - create_image_file
    *                  read_image_array
    *                  write_image_array
    *                  get_image_size
    *                  allocate_image_array
    *                  free_image_array
    *
    *  Modifications:
    *     5 April 1998 - created
    *     22 September 1998 - modified to work with 
    *           all I O routines in imageio.c.
    *
    *************************************************/

#include "cips.h"
#define EIGHT 8



main(argc, argv)
   int argc;
   char *argv[];
{

   char  cover_image_name[80], 
         message_image_name[80];
   int   hide    = 0,
         i, 
         j, 
         lsb,
         n,
         uncover = 0;

   long  clength, 
         mlength, 
         cwidth,
         mwidth;

   short **the_image;
   short **out_image;

      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc < 5){
      stega_show_usage();
      exit(0);
   }

   if(strcmp(argv[1], "-h") == 0){
      hide    = 1;
      uncover = 0;
   }
   if(strcmp(argv[1], "-u") == 0){
      hide    = 0;
      uncover = 1;
   }
   if(  (hide    == 0)   &&
        (uncover == 0)  ){
      printf("\nNiether hiding nor uncovering");
      printf("\nSo, quitting");
      exit(1);
   }  /* ends if */

   strcpy(cover_image_name, argv[2]);
   strcpy(message_image_name, argv[3]);
   n = atoi(argv[4]);


      /******************************************
      *
      * Hide the cover image in the message image.
      *
      ******************************************/


   if(hide){
      if(does_not_exist(cover_image_name)){
         printf("\n%s does not exist, quitting", 
         cover_image_name);
      }
      if(does_not_exist(message_image_name)){
         printf("\n%s does not exist, quitting", 
         message_image_name);
      }

         /******************************************
         *
         *   Ensure both images have the same height
         *   and the cover image is eight times as
         *   wide as the message image.
         *   Also determine if the bit order is lsb
         *   first or not.
         *
         ******************************************/

      get_image_size(cover_image_name, 
                     &clength, &cwidth);
      get_image_size(message_image_name, 
                     &mlength, &mwidth);
      
      if(mlength != clength){
         printf("\n\nmlength NOT EQUAL TO clength");
         printf("\nQUITING");
         exit(2);
      }  /* ends if length not equal */

      if(cwidth != (n*mwidth)){
         printf("\nCover image not wide enough");
         printf("\nQUITING");
         exit(3);
      }  /* ends if cover image not wide enough */

      lsb = get_lsb(cover_image_name);

         /******************************************
         *
         *   Allocate the two image arrays.
         *   Read the cover and message images and
         *   hide the message image.
         *
         ******************************************/

      the_image = allocate_image_array(
                     clength, cwidth);
      out_image = allocate_image_array(
                     mlength, mwidth);
      read_image_array(cover_image_name, the_image);
      read_image_array(message_image_name, out_image);

      hide_image(the_image, out_image,
                 mlength, mwidth,
                 clength, cwidth,
                 lsb, n);
      write_image_array(cover_image_name, the_image);
   }  /* ends if hide */



      /******************************************
      *
      *   Uncover the cover image from the 
      *   message image.
      *
      ******************************************/

   if(uncover){
printf("\nMAIN> Uncover");

      if(does_not_exist(cover_image_name)){
         printf("\n%s does not exist, quitting", 
         cover_image_name);
      }  /* ends if does_not_exist */

         /******************************************
         *
         *   Create the message image to be the
         *   correct size.
         *
         ******************************************/

      get_image_size(cover_image_name, 
                     &clength, &cwidth);
      mlength = clength;
      mwidth  = cwidth/n;
      create_resized_image_file(cover_image_name,
                                message_image_name, 
                                mlength, mwidth); 
      lsb = get_lsb(cover_image_name);

         /******************************************
         *
         *   Allocate the two image arrays.
         *   Read the cover image and uncover
         *   the message image.
         *
         ******************************************/

      the_image = allocate_image_array(
                     clength, cwidth);
      out_image = allocate_image_array(
                     mlength, mwidth);
      read_image_array(cover_image_name, the_image);
      uncover_image(the_image, out_image,
                    mlength, mwidth,
                    clength, cwidth,
                    lsb, n);
      write_image_array(message_image_name, out_image);

   }  /* ends if uncover */


   free_image_array(the_image, clength);
   free_image_array(out_image, mlength);

}  /* ends main  */



   /*********************************************
   *
   *   hide_image(...
   *
   *   This routine hides the message image in
   *   the cover image.  Loop through the pixels
   *   in the message image and call hide_pixels
   *   for every pixel in the message image.
   *
   *********************************************/


int hide_image(cover_image,
               message_image,
               mlength,
               mwidth,
               clength,
               cwidth,
               lsb,
               n)
   int   lsb, n;
   long  clength, cwidth, mlength, mwidth;
   short **cover_image, 
         **message_image;
{
   char response[80];
   int h_counter        = 0;


   for(h_counter=0; h_counter<mwidth; h_counter++){
      hide_pixels(cover_image,
                  message_image,
                  h_counter,
                  h_counter*n,
                  lsb,
                  n,
                  mlength);
   }  /* ends loop over h_counter */

}  /* ends hide_image */




   /*********************************************
   *
   *   hide_pixels(..
   *
   *   This routine hides the bits in a pixel
   *   from the message image into the least
   *   significant bit of eight pixels in the
   *   cover image.
   *
   *   Do this one operation for every row of
   *   pixels in the message and cover images.
   *
   *********************************************/

int hide_pixels(cover_image, 
                message_image,
                mie, 
                cie,
                lsb,
                n,
                mlength)
   int   cie, lsb, mie, n;
   long  mlength;
   short **cover_image, 
         **message_image;
{
   char result,
        new_message,
        sample;

   char mask1[EIGHT] =  {0x01,  /* 0000 0001 */
                         0x02,  /* 0000 0010 */
                         0x04,  /* 0000 0100 */
                         0x08,  /* 0000 1000 */
                         0x10,  /* 0001 0000 */
                         0x20,  /* 0010 0000 */
                         0x40,  /* 0100 0000 */
                         0x80}; /* 1000 0000 */


   char mask2[EIGHT] = {0xFE,  /* 1111 1110 */
                        0xFD,  /* 1111 1101 */
                        0xFB,  /* 1111 1011 */
                        0xF7,  /* 1111 0111 */
                        0xEF,  /* 1110 1111 */
                        0xDF,  /* 1101 1111 */
                        0xBF,  /* 1011 1111 */
                        0x7F}; /* 0111 1111 */


   int c_counter,
       i, j;

printf("\nHP> mie=%d   cie=%d   lsb=%d", mie, cie, lsb);

   for(i=0; i<mlength; i++){
      c_counter = 0;
      sample    = message_image[i][mie];

      for(j=n-1; j>-1; j--){

         /***********************************
         *
         *   Find out if the jth bit is 
         *   a 1 or 0.  If it is non-zero,
         *   set the LSB of the message image's
         *   pixel.  Else, clear that LSB.
         *
         ***********************************/

         new_message = cover_image[i][cie+c_counter];
         result      = sample & mask1[j];

         if(result != 0x00){ /* set lsb */
            if(lsb)
               new_message = new_message | mask1[0];
            else
               new_message = new_message | mask1[EIGHT];
         } /* ends if set lsb */

         else{ /* clear lsb */
            if(lsb)
               new_message = new_message & mask2[0];
            else
               new_message = new_message & mask2[EIGHT];
         }  /* ends if clear lsb */

         cover_image[i][cie+c_counter] = new_message;
         c_counter++;

      }  /* ends loop over j */
   }  /* ends loop over i */
}  /* ends hide_pixels */



   /*********************************************
   *
   *   uncover_image(...
   *
   *   This routine pulls the message image out
   *   of the cover image (the opposite of 
   *   the cover_image routine).
   *
   *********************************************/

int uncover_image(cover_image,
                  message_image,
                  mlength,
                  mwidth,
                  clength,
                  cwidth,
                  lsb,
                  n)
   int   lsb, n;
   long  clength, cwidth, mlength, mwidth;
   short **cover_image, 
         **message_image;
{
   int h_counter;

   for(h_counter=0; h_counter<mwidth; h_counter++){
      uncover_pixels(cover_image, 
                     message_image,
                     h_counter,
                     h_counter*n,
                     lsb,
                     n,
                     mlength);
   }  /* ends loop over h_counter */

}  /* ends uncover_image */



   /*********************************************
   *
   *   uncover_pixels(...
   *
   *   This routine pulls the eight bits that
   *   make up a pixel in the message image
   *   out of the LSB of eight pixels in the
   *   cover image.  It is the opposite of the
   *   cover_pixels routine.
   *
   *********************************************/

int uncover_pixels(cover_image, 
                message_image,
                mie, 
                cie,
                lsb,
                n,
                mlength)
   int   cie, lsb, mie, n;
   long  mlength;
   short **cover_image, 
         **message_image;
{
   char result,
        new_message,
        sample;

   char mask1[EIGHT] =  
                        {0x80,  /* 1000 0000 */
                         0x40,  /* 0100 0000 */
                         0x20,  /* 0010 0000 */
                         0x10,  /* 0001 0000 */
                         0x08,  /* 0000 1000 */
                         0x04,  /* 0000 0100 */
                         0x02,  /* 0000 0010 */
                         0x01}; /* 0000 0001 */
   char mask2[EIGHT] = 
                       {0x7F,  /* 0111 1111 */
                        0xBF,  /* 1011 1111 */
                        0xDF,  /* 1101 1111 */
                        0xEF,  /* 1110 1111 */
                        0xF7,  /* 1111 0111 */
                        0xFB,  /* 1111 1011 */
                        0xFD,  /* 1111 1101 */
                        0xFE}; /* 1111 1110 */

   int c, c_counter, i, j;
printf("\nUP> mie=%d   cie=%d   lsb=%d", 
mie, cie, lsb);

      /*************************************
      *
      *  If a pixel in the cover image is 
      *  odd, its lsb has been set, so 
      *  the corresponding bit in the 
      *  message image should be set.
      *
      *************************************/

   for(i=0; i<mlength; i++){
      c = n-1;
      new_message = 0x00;
      for(j=0; j<n; j++){
         if(is_odd(cover_image[i][cie+j])){
            /* set bit c */
            if(lsb)
               new_message = new_message | mask1[j];
            else
               new_message = new_message | mask1[c];
         }  /* ends if is_odd */
         c--;
      }  /* ends loop over j */
      message_image[i][mie] = new_message;
   }  /* ends loop over i */
}  /* ends uncover_pixels */



   /*********************************************
   *
   *   is_odd(...
   *
   *   This routine determines if a short is 
   *   an odd number.  If it is, this routine
   *   returns a 1, else it returns a 0.
   *
   *********************************************/

int is_odd(number)
   short number;
{
   int result = 0;
   result     = number % 2;
   return(result);
}  /* ends is_odd */


stega_show_usage()
{
printf("\n\nNot enough parameters:");
printf("\n");
printf("   "
"\nstega -h cover-image-name message-image-name n"
"\n       to hide the message image in the cover image"
"\n                 or"
"\nstega -u cover-image-name message-image-name n"
"\n       to uncover the cover image from "
"the message image");
}
