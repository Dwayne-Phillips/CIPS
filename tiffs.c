
   /*************************** 
   * 
   *   tiffs.c 
   *   COMPOSITE FILE COMPRISING: 
   *   rtiff.c 
   *   wtiff.c 
   *   tiff.c 
   * 
   ***************************\ 



       /*********************************************
       *
       *  file d:\cips\rtiff.c
       *
       *  Functions: This file contains
       *          read_tiff_image
       *          read_line
       *          seek_to_first_line
       *          seek_to_end_of_line
       *
       *  Purpose:
       *  These functions read a TIFF image and insert
       *  the data into a ROWSxCOLS array of short.
       *
       *       NOTE: The fseek constants are
       *             0=SEEK_SET = beginning of file
       *             1=SEEK_CUR = current offset
       *             2=SEEK_END = end of file
       *
       *  External Calls:
       *  tiff.c - read_tiff_header
       *
       *  Modifications:
       *       25 June 1990 - created
       *       27 March 1993 - use fopen, fread, fseek
       *           instead of the earlier open, read,
       *           seek, etc.
       *
       **********************************************/

#include "cips.h"





read_tiff_image(image_file_name, array, il, ie, ll, le)
      char   image_file_name[];
      int    il, ie, ll, le;
      short   array[ROWS][COLS];
{
   char  buffer[COLS],
         rep[80];
   int   bytes_read,
         closed,
         position,
         i;
   FILE  *image_file;
   float a;
   long  line_length, offset;

   struct tiff_header_struct image_header;

   read_tiff_header(image_file_name, &image_header);

      /***********************************************
      *
      *   Procedure:
      *   Seek to the strip offset where the data begins.
      *   Seek to the first line you want.
      *   Loop over the lines you want to read:
      *      Seek to the first element of the line.
      *      Read the line.
      *      Seek to the end of the data in that line.
      *
      ************************************************/

   image_file = fopen(image_file_name, "rb");
   if(image_file != NULL){
      position = fseek(image_file, 
                       image_header.strip_offset, 
                       SEEK_SET);
      position = seek_to_first_line(image_file, 
                                    &image_header, il);

      for(i=0; i<(ll-il); i++){
         offset       = (ie-1)/
                        (8/image_header.bits_per_pixel);
         position     = fseek(image_file, offset, 
                              SEEK_CUR);
         bytes_read   = read_line(image_file, array, 
                                  i, &image_header, 
                                  ie, le);
         position     = seek_to_end_of_line(image_file, 
                                 le, &image_header);
         position     = fseek(image_file, 1, 
                              SEEK_CUR); 
      }  /* ends loop over i  */

      closed = fclose(image_file);
   }  /* ends if file opened ok */
   else{
      printf("\nRTIFF.C> ERROR - cannot open "
             "tiff file");
   }

}  /*  ends read_tiff_image */




       /**********************************************
       *
       *   read_line(...
       *
       *   This function reads bytes from the TIFF 
       *   file into a buffer, extracts the numbers 
       *   from that buffer, and puts them into a 
       *   ROWSxCOLS array of shorts. The process 
       *   depends on the number of bits per pixel used 
       *   in the file (4 or 8).
       *
       **********************************************/

read_line(image_file, array, line_number, 
          image_header, ie, le)
   FILE   *image_file;
   int    ie, le, line_number;
   short  array[ROWS][COLS];
   struct tiff_header_struct *image_header;
{
   char  buffer[COLS], first, second;
   float a, b;
   int bytes_read, i;
   unsigned int bytes_to_read;
   union short_char_union scu;

   for(i=0; i<COLS; i++)
      buffer[i] = '\0';

        /********************************************
        *
        *   Use the number of bits per pixel to 
        *   calculate how many bytes to read.
        *
        ********************************************/

   bytes_to_read = (le-ie)/
                   (8/image_header->bits_per_pixel);
   bytes_read    = fread(buffer, 1, bytes_to_read, 
                         image_file);

   for(i=0; i<bytes_read; i++){

        /*********************************************
        *
        *   Use unions defined in cips.h to stuff bytes
        *   into shorts.
        *
        **********************************************/

      if(image_header->bits_per_pixel == 8){
       scu.s_num          = 0;
       scu.s_alpha[0]        = buffer[i];
       array[line_number][i] = scu.s_num;
      }  /* ends if bits_per_pixel == 8 */

      if(image_header->bits_per_pixel == 4){

       scu.s_num             = 0;
       second                = buffer[i] & 0X000F;
       scu.s_alpha[0]        = second;
       array[line_number][i*2+1] = scu.s_num;

       scu.s_num             = 0;
       first                 = buffer[i] >> 4;
       first                 = first & 0x000F;
       scu.s_alpha[0]        = first;
       array[line_number][i*2] = scu.s_num;

      }  /* ends if bits_per_pixel == 4 */

   }  /*  ends loop over i  */

   return(bytes_read);

}  /* ends read_line  */





       /*********************************************
       *
       *   seek_to_first_line(...
       *
       **********************************************/

seek_to_first_line(image_file, image_header, il)
   FILE   *image_file;
   int    il;
   struct tiff_header_struct *image_header;
{
   long offset;
   int  position;

   offset   = (il-1)*image_header->image_width/
             (8/image_header->bits_per_pixel);
      /* seek from current position */
   position = fseek(image_file, offset, SEEK_CUR);
   return(position);
}  /* ends seek_to_first_line */





       /**********************************************
       *
       *   seek_to_end_of_line(...
       *
       ***********************************************/

seek_to_end_of_line(image_file, le, image_header)
   FILE   *image_file;
   int    le;
   struct tiff_header_struct *image_header;
{
   long  offset;
   int   position;

   offset   = (image_header->image_width-le)/
             (8/image_header->bits_per_pixel);
   position = fseek(image_file, offset, SEEK_CUR);
   return(position);
}  /* ends seek_to_end_of_line         */

       /**********************************************
       *
       *  file d:\cips\wtiff.c
       *
       *  Functions: This file contains
       *      create_file_if_needed
       *      create_allocate_tiff_file
       *      write_array_into_tiff_image
       *      write_line
       *      insert_short_into_buffer
       *      insert_long_into_buffer
       *      round_off_image_size
       *      does_not_exist
       *
       *  Purpose:
       *     These functions create TIFF image files 
       *     on disk and insert a ROWSxCOLS array
       *     into a tiff image already stored on disk.
       *
       *  External Calls:
       *     rtiff.c - seek_to_first_line
       *               seek_to_end_of_line
       *     tiff.c - read_tiff_header
       *
       *  Modifications:
       *     29 January 1991 - created
       *     28 March 1993 - replaced open, lseek
       *         etc. with fopen, fseek, etc.
       *     10 May 1993 - added a number of tags
       *         to make the TIFF files I create
       *         TIFF 6.0 Gray Scale image compliant.
       *
       *********************************************/






       /**********************************************
       *
       *   create_file_if_needed(...
       *
       *   This function creates a file on disk
       *   if it does not exist.  The out file is
       *   patterned after the in file.
       *
       ***********************************************/

create_file_if_needed(in_name, out_name, out_image)
   char in_name[], out_name[];
   short out_image[ROWS][COLS];
{
   int    length, width;
   struct tiff_header_struct image_header;

   if(does_not_exist(out_name)){
      printf("\n\n output file does not exist %s",
               out_name);
      read_tiff_header(in_name, &image_header);
      round_off_image_size(&image_header,
                           &length, &width);
      image_header.image_length = length*ROWS;
      image_header.image_width  = width*COLS;
      create_allocate_tiff_file(out_name, &image_header,
                                out_image);
   }  /* ends if does_not_exist */
}  /* ends create_file_if_needed */





   /**********************************************
   *
   *   create_alllocate_tiff_file(...
   *
   *   This function creates a file on disk that will be
   *   large enough to hold a tiff image.  The input
   *   tiff_header_struct describes the desired tiff file.
   *   This function writes the tiff header and then
   *   writes a blank image array out to disk the proper
   *   number of times.  This has the effect of allocating
   *   the correct number of bytes on the disk.
   *
   *   There will be 18 entries in the IFD.
   *   The image data will begin at byte 296.
   *   I will use LSB first data.
   *   I will have one strip for the entire image.
   *   Black is zero.
   *   The component values for the image are CHUNKY
   *      (Planer configuration = 1).
   *
   ***************************************************/


create_allocate_tiff_file(file_name, 
                          image_header, image)
   char   file_name[];
   short  image[ROWS][COLS];
   struct tiff_header_struct *image_header;
{
   char  buffer[12], long_buffer[50];
   FILE  *image_file;
   int   bytes_written,
         i,
         j,
         l,
         w;

   long  k;



      /***************************************
      *
      *   Create the image file in binary mode
      *   for both reading and writing.
      *
      ****************************************/

   image_file = fopen(file_name, "wb");

      /***************************************
      *
      *   Write out the first 8 bytes of the
      *   header.  The meaning of the
      *   bytes (HEX) is:
      *      0-1 = 49 49 - LSB first
      *      2-3 = 2A 00 - version #
      *      4-7 = 08 00 00 00 - go to offset
      *           8 for the first
      *           Image File
      *           Directory
      *
      ****************************************/

   buffer[0] = 0x49;
   buffer[1] = 0x49;
   buffer[2] = 0x2A;
   buffer[3] = 0x00;
   buffer[4] = 0x08;
   buffer[5] = 0x00;
   buffer[6] = 0x00;
   buffer[7] = 0x00;

   bytes_written = fwrite(buffer, 1, 8, image_file);

   printf("\n wrote %d bytes", bytes_written);

      /***************************************
      *
      *   Write out the first 2 bytes of the
      *   Image File Directory.  These tell
      *   the number of entries in the IFD.
      *
      ****************************************/

   buffer[0] = 0x12;
   buffer[1] = 0x00;
   bytes_written = fwrite(buffer, 1, 2, image_file);

   printf("\n wrote %d bytes", bytes_written);

      /***************************************
      *
      *   Write out the entries into the
      *   Image File Directory.
      *
      ****************************************/


      /* New Subfile Type */
   buffer[0]  = 0xFE;
   buffer[1]  = 0x00;
   buffer[2]  = 0x03;
   buffer[3]  = 0x00;
   buffer[4]  = 0x01;
   buffer[5]  = 0x00;
   buffer[6]  = 0x00;
   buffer[7]  = 0x00;
   buffer[8]  = 0x00;
   buffer[9]  = 0x00;
   buffer[10] = 0x00;
   buffer[11] = 0x00;
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Subfile Type */
   buffer[0]  = 0xFF;
   buffer[1]  = 0x00;
   buffer[2]  = 0x03;
   buffer[3]  = 0x00;
   buffer[4]  = 0x01;
   buffer[5]  = 0x00;
   buffer[6]  = 0x00;
   buffer[7]  = 0x00;
   buffer[8]  = 0x01;
   buffer[9]  = 0x00;
   buffer[10] = 0x00;
   buffer[11] = 0x00;
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Image Width */
   insert_short_into_buffer(buffer, 0, 256);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 
                     image_header->image_width);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Image Length */
   insert_short_into_buffer(buffer, 0, 257);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 
                     image_header->image_length);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Bits Per Sample */
   insert_short_into_buffer(buffer, 0, 258);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 
                     image_header->bits_per_pixel);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Compression - None */
   insert_short_into_buffer(buffer, 0, 259);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 1);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Photometric Interpretation */
      /* set to 1 because BLACK is ZERO */
   insert_short_into_buffer(buffer, 0, 262);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 1);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Strip Offset */
      /* start after software name at 296 */
   insert_short_into_buffer(buffer, 0, 273);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 296);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Samples per Pixel */
   insert_short_into_buffer(buffer, 0, 277);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 1);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* clear buffer */
   for(i=0; i<12; i++) buffer[i] = 0x00;

      /* Rows Per Strip 1 strip for the entire image */
      /* use 2E32 - 1, which is max */
   insert_short_into_buffer(buffer, 0, 278);
   insert_short_into_buffer(buffer, 2, 4);
   insert_short_into_buffer(buffer, 4, 1);
   insert_long_into_buffer(buffer, 8, 4294967295);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Strip Byte Counts */
      /* this = image width times length */
   insert_short_into_buffer(buffer, 0, 279);
   insert_short_into_buffer(buffer, 2, 4);
   insert_short_into_buffer(buffer, 4, 1);
   insert_long_into_buffer(buffer, 8, 
    (long)(image_header->image_length *
           image_header->image_width));
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Min Sample Value */
   insert_short_into_buffer(buffer, 0, 280);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 0);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Max Sample Value */
   insert_short_into_buffer(buffer, 0, 281);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   if(image_header->bits_per_pixel == 8)
      insert_short_into_buffer(buffer, 8, 255);
   else
      insert_short_into_buffer(buffer, 8, 15);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* X Resolution */
      /* Store the 8 bytes for this value
         starting at 230 */
   insert_short_into_buffer(buffer, 0, 282);
   insert_short_into_buffer(buffer, 2, 5);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 230);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Y Resolution */
      /* Store the 8 bytes for this value
         starting at 238 */
   insert_short_into_buffer(buffer, 0, 283);
   insert_short_into_buffer(buffer, 2, 5);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 238);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);



      /* clear buffer */
   for(i=0; i<12; i++) buffer[i] = 0x00;

      /* Planer Configuration */
      /* chunky */
   insert_short_into_buffer(buffer, 0, 284);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 1);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Resolution Unit */
      /* inches */
   insert_short_into_buffer(buffer, 0, 296);
   insert_short_into_buffer(buffer, 2, 3);
   insert_short_into_buffer(buffer, 4, 1);
   insert_short_into_buffer(buffer, 8, 2);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Software */
      /* Put this a 246, 50 bytes */
   insert_short_into_buffer(buffer, 0, 305);
   insert_short_into_buffer(buffer, 2, 2);
   insert_short_into_buffer(buffer, 4, 50);
   insert_short_into_buffer(buffer, 8, 246);
   bytes_written = fwrite(buffer, 1, 12, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Offset to next IFD (0 means no more IFD's) */
   for(i=0; i<12; i++) buffer[i] = 0x00;
   bytes_written = fwrite(buffer, 1, 4, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* clear buffer */
   for(i=0; i<12; i++) buffer[i] = 0x00;

      /* Now store the X Resolution
         first long is numerator
         second long is denominator */
   insert_long_into_buffer(buffer, 0, 300L);
   insert_long_into_buffer(buffer, 4, 1L);
   bytes_written = fwrite(buffer, 1, 8, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Now store the Y Resolution
         first long is numerator
         second long is denominator */
   insert_long_into_buffer(buffer, 0, 300L);
   insert_long_into_buffer(buffer, 4, 1L);
   bytes_written = fwrite(buffer, 1, 8, image_file);
   printf("\n wrote %d bytes", bytes_written);


      /* Now store the software tag */
   for(i=0; i<50; i++) long_buffer[i] = '\0';
   strcpy(long_buffer,
   "Dwayne Phillips C Image Processing System 1993");
   long_buffer[46] = '\0';
   long_buffer[47] = '\0';
   long_buffer[48] = '\0';
   long_buffer[49] = '\0';
   bytes_written = fwrite(long_buffer, 1, 50, 
                          image_file);
   printf("\n wrote %d bytes", bytes_written);
   printf("\n%s", long_buffer);



      /***************************************
      *
      *   Now write the image data.
      *
      ****************************************/

   printf("\n length is %ld", 
          image_header->image_length);
   printf("\n width is %ld", 
          image_header->image_width);

   round_off_image_size(image_header, &l, &w);
   k = l * w;

   if(image_header->bits_per_pixel == 8)
      k = k/2;
   else
      k = k/4;
   k++;

   for(i=0; i<ROWS; i++)
      for(j=0; j<COLS; j++)
        image[i][j] = 0;

   j = sizeof(short) * ROWS * COLS;

   for(i=0; i<k; i++){
      bytes_written = fwrite(image, 1, j, image_file);
      printf("\n wrote %d bytes", bytes_written);
   }


   fclose(image_file);

}  /* ends create_allocate_tiff_file */





       /*********************************************
       *
       * write_array_into_tiff_file(...
       *
       * This function takes an array of shorts and 
       * writes them into an existing tiff image file.
       *
       **********************************************/

write_array_into_tiff_image(image_file_name, array,
                            il, ie, ll, le)

        char    image_file_name[];
        int     il, ie, ll, le;
        short   array[ROWS][COLS];
{

   char  buffer[COLS];
   FILE  *image_file;
   int   bytes_written,
         closed,
         i,
          position,
         written;

   float a;

   long  line_length,
         offset;

   struct tiff_header_struct image_header;



   read_tiff_header(image_file_name, &image_header);


      /***********************************************
      *
      *   Procedure:
      *   Seek to the strip offset where the data begins.
      *   Seek to the first line you want.
      *   Loop over the lines you want to write.
      *      Seek to the first element of the line.
      *      Write the line.
      *      Seek to the end of the data in that line.
      *
      ************************************************/

   image_file = fopen(image_file_name, "rb+");
   position   = fseek(image_file,
                      image_header.strip_offset, 
                      SEEK_SET);
   position   = seek_to_first_line(image_file,
                                   &image_header, il);

   for(i=0; i<(ll-il); i++){
      offset        = (ie-1)/
                      (8/image_header.bits_per_pixel);
      position      = fseek(image_file, offset, 
                            SEEK_CUR);
      bytes_written = write_line(image_file, array,
                                 i, &image_header, 
                                 ie, le);
      position      = seek_to_end_of_line(image_file,
                                          le, 
                                          &image_header);
      position      = fseek(image_file, 1, SEEK_CUR);
   }  /* ends loop over i  */

   closed = fclose(image_file);

}  /*  ends write_array_into_tiff_image */




       /*********************************************
       *
       *   write_line(...
       *
       *   This function takes an array of shorts, 
       *   extracts the numbers and puts them into 
       *   a buffer, then writes this buffer into a 
       *   tiff file on disk. The process depends on 
       *   the number of bits per pixel used in the 
       *   file (4 or 8).
       *
       **********************************************/

write_line(image_file, array, line_number, 
           image_header, ie, le)
   FILE   *image_file;
   int    ie, le, line_number;
   short  array[ROWS][COLS];
   struct tiff_header_struct *image_header;
{
   char     buffer[COLS], first, second;
   float    a, b;
   int      bytes_written, i;
   unsigned int bytes_to_write;
   union    short_char_union scu;

   for(i=0; i<COLS; i++)
      buffer[i] = '\0';

   bytes_to_write = (le-ie)/
                    (8/image_header->bits_per_pixel);

   for(i=0; i<bytes_to_write; i++){

        /**********************************************
        *
        *   Use unions defined in cips.h to stuff shorts
        *   into bytess.
        *
        **********************************************/

      if(image_header->bits_per_pixel == 8){
       scu.s_num = 0;
       scu.s_num = array[line_number][i];
       buffer[i] = scu.s_alpha[0];
      }  /* ends if bits_per_pixel == 8 */


      if(image_header->bits_per_pixel == 4){

       scu.s_num = 0;
       scu.s_num = array[line_number][i*2];
       first     = scu.s_alpha[0] << 4;

       scu.s_num = 0;
       scu.s_num = array[line_number][i*2];
       second    = scu.s_alpha[0] & 0X000F;

       buffer[i] = first | second;
      }  /* ends if bits_per_pixel == 4 */

   }  /*  ends loop over i  */


   bytes_written = fwrite(buffer, 1, bytes_to_write, 
                          image_file);

   return(bytes_written);

}  /* ends write_line  */






   /***************************************
   *
   *   insert_short_into_buffer(...
   *
   *   This inserts a two byte short into a
   *   buffer of characters.  It does this
   *   is LSB order.
   *
   ***************************************/


insert_short_into_buffer(buffer, start, number)
    char  buffer[];
    int   start;
    short number;
{
    union short_char_union lsu;

    lsu.s_num       = number;
    buffer[start+0] = lsu.s_alpha[0];
    buffer[start+1] = lsu.s_alpha[1];

}  /* ends insert_short_into_buffer */




   /***************************************
   *
   *   insert_long_into_buffer(...
   *
   *   This inserts a four byte long into a
   *   buffer of characters.  It does this
   *   is LSB order.
   *
   ***************************************/



insert_long_into_buffer(buffer, start, number)
    char buffer[];
    int  start;
    long number;
{
    union long_char_union lsu;

    lsu.l_num       = number;
    buffer[start+0] = lsu.l_alpha[0];
    buffer[start+1] = lsu.l_alpha[1];
    buffer[start+2] = lsu.l_alpha[2];
    buffer[start+3] = lsu.l_alpha[3];

}  /* ends insert_short_into_buffer */




   /***************************************
   *
   *   round_off_image_size(...
   *
   *   This takes the image header and rounds
   *   it off to a multiple of ROWS and COLS.
   *   e.g. if width=123 it returns 1.
   *
   ***************************************/


round_off_image_size(image_header, length, width)
    int    *length, *width;
    struct tiff_header_struct *image_header;
{
   *length = (ROWS-10 + image_header->image_length)/ROWS;
   *width  = (COLS-10 + image_header->image_width)/COLS;
} /* ends round_off_image_size */





   /***********************************************
    *
    *    does_not_exist(...
    *
    *    This function checks the disk to see if
    *    a file exists.  If the file is there this
    *    function returns a 0, if it does not exist
    *    this function returns a 1.
    *
    ***********************************************/

does_not_exist(file_name)
    char file_name[];
{
   FILE *image_file;
   int  result;

   result = 1;
   image_file = fopen(file_name, "rb");
   if(image_file != NULL){
      result = 0;
      fclose(image_file);
   }
   return(result);
}  /* ends does_not_exist */


       /*********************************************
       *
       *   file d:\cips\tiff.c
       *
       *   Functions: This file contains
       *     read_tiff_header
       *     extract_long_from_buffer
       *     extract_short_from_buffer
       *
       *   Purpose:
       *     This file contains the subroutines that 
       *     read the tiff files header information.
       *
       *   External Calls:
       *      none
       *
       *   Modifications:
       *      23 June 1990 - created
       *      28 March 1993 - using fopen, fread, fseek
       *          instead of my_open, my_read, lseek.
       *
       ************************************************/



       /***********************************************
       *
       *   read_tiff_header(...
       *
       *   This function reads the header of a TIFF 
       *   file and places the needed information into
       *   the struct tiff_header_struct.
       *
       ***********************************************/

read_tiff_header(file_name, image_header)
   char file_name[];
   struct tiff_header_struct *image_header;
{
   char buffer[12], response[80];

   FILE *image_file;

   int  bytes_read,
        closed,
        i,
        j,
        lsb,
        not_finished,
        position;

   long bits_per_pixel,
        image_length,
        image_width,
        length_of_field,
        offset_to_ifd,
        strip_offset,
        subfile,
        value;

   short entry_count,
         field_type,
         s_bits_per_pixel,
         s_image_length,
         s_image_width,
         s_strip_offset,
         tag_type;

   image_file = fopen(file_name, "rb");
   if(image_file != NULL){

        /*************************************
        *
        *   Determine if the file uses MSB
        *   first or LSB first
        *
        *************************************/

   bytes_read = fread(buffer, 1, 8, image_file);

   if(buffer[0] == 0x49)
      lsb = 1;
   else
      lsb = 0;

        /*************************************
        *
        *   Read the offset to the IFD
        *
        *************************************/

   extract_long_from_buffer(buffer, lsb, 4, 
                            &offset_to_ifd);

   not_finished = 1;
   while(not_finished){

        /*************************************
        *
        *   Seek to the IFD and read the
        *   entry_count, i.e. the number of
        *   entries in the IFD.
        *
        *************************************/

      position   = fseek(image_file, offset_to_ifd, 
                         SEEK_SET);
      bytes_read = fread(buffer, 1, 2, image_file);
      extract_short_from_buffer(buffer, lsb, 0, 
                                &entry_count);

        /***************************************
        *
        *   Now loop over the directory entries.
        *   Look only for the tags we need.  These
        *   are:
        *     ImageLength
        *     ImageWidth
        *     BitsPerPixel(BitsPerSample)
        *     StripOffset
        *
        *****************************************/

      for(i=0; i<entry_count; i++){
       bytes_read = fread(buffer, 1, 12, image_file);
       extract_short_from_buffer(buffer, lsb, 0, 
                                 &tag_type);

       switch(tag_type){

          case 255: /* Subfile Type */
             extract_short_from_buffer(buffer, lsb, 2,
                                       &field_type);
             extract_short_from_buffer(buffer, lsb, 4,
                                    &length_of_field);
             extract_long_from_buffer(buffer, lsb, 8, 
                                      &subfile);
             break;

          case 256: /* ImageWidth */
             extract_short_from_buffer(buffer, lsb, 2, 
                                       &field_type);
             extract_short_from_buffer(buffer, lsb, 4,
                                    &length_of_field);
             if(field_type == 3){
              extract_short_from_buffer(buffer, lsb, 8,
                                     &s_image_width);
              image_width = s_image_width;
             }
             else
              extract_long_from_buffer(buffer, lsb, 8, 
                                       &image_width);
             break;

          case 257: /* ImageLength */
             extract_short_from_buffer(buffer, lsb, 2, 
                                       &field_type);
             extract_short_from_buffer(buffer, lsb, 4,
                                    &length_of_field);
             if(field_type == 3){
              extract_short_from_buffer(buffer, lsb, 8,
                                    &s_image_length);
              image_length = s_image_length;
             }
             else
              extract_long_from_buffer(buffer, lsb, 8,
                                       &image_length);
             break;

          case 258: /* BitsPerSample */
             extract_short_from_buffer(buffer, lsb, 2, 
                                       &field_type);
             extract_short_from_buffer(buffer, lsb, 4,
                                    &length_of_field);
             if(field_type == 3){
              extract_short_from_buffer(buffer, lsb, 8,
                                   &s_bits_per_pixel);
              bits_per_pixel = s_bits_per_pixel;
             }
             else
              extract_long_from_buffer(buffer, lsb, 8,
                                    &bits_per_pixel);
             break;

          case 273: /* StripOffset */
             extract_short_from_buffer(buffer, lsb, 2, 
                                       &field_type);
             extract_short_from_buffer(buffer, lsb, 4,
                                    &length_of_field);
             if(field_type == 3){
              extract_short_from_buffer(buffer, lsb, 8,
                                    &s_strip_offset);
              strip_offset = s_strip_offset;
             }
             else
              extract_long_from_buffer(buffer, lsb, 8,
                                       &strip_offset);
             break;

          default:
             break;

       }  /* ends switch tag_type */

      }  /* ends loop over i directory entries */

      bytes_read = fread(buffer, 1, 4, image_file);
      extract_long_from_buffer(buffer, lsb, 0, 
                               &offset_to_ifd);
      if(offset_to_ifd == 0) not_finished = 0;

   }  /* ends while not_finished */


   image_header->lsb                = lsb;
   image_header->bits_per_pixel = bits_per_pixel;
   image_header->image_length       = image_length;
   image_header->image_width        = image_width;
   image_header->strip_offset       = strip_offset;

   closed = fclose(image_file);
   }  /* ends if file opened ok */
   else{
      printf("\n\nTIFF.C> ERROR - could not open "
             "tiff file");
   }
}  /* ends read_tiff_header */





   /****************************************
   *
   *   extract_long_from_buffer(...
   *
   *   This takes a four byte long out of a
   *   buffer of characters.
   *
   *   It is important to know the byte order
   *   LSB or MSB.
   *
   ****************************************/

extract_long_from_buffer(buffer, lsb, start, number)
   char  buffer[];
   int       lsb, start;
   long  *number;
{
   int i;
   union long_char_union lcu;

   if(lsb == 1){
      lcu.l_alpha[0] = buffer[start+0];
      lcu.l_alpha[1] = buffer[start+1];
      lcu.l_alpha[2] = buffer[start+2];
      lcu.l_alpha[3] = buffer[start+3];
   }  /* ends if lsb = 1 */

   if(lsb == 0){
      lcu.l_alpha[0] = buffer[start+3];
      lcu.l_alpha[1] = buffer[start+2];
      lcu.l_alpha[2] = buffer[start+1];
      lcu.l_alpha[3] = buffer[start+0];
   }  /* ends if lsb = 0      */

   *number = lcu.l_num;


}  /* ends extract_long_from_buffer */






extract_ulong_from_buffer(buffer, lsb, start, number)
   char  buffer[];
   int       lsb, start;
   unsigned long  *number;
{
   int i;
   union ulong_char_union lcu;

   if(lsb == 1){
      lcu.l_alpha[0] = buffer[start+0];
      lcu.l_alpha[1] = buffer[start+1];
      lcu.l_alpha[2] = buffer[start+2];
      lcu.l_alpha[3] = buffer[start+3];
   }  /* ends if lsb = 1 */

   if(lsb == 0){
      lcu.l_alpha[0] = buffer[start+3];
      lcu.l_alpha[1] = buffer[start+2];
      lcu.l_alpha[2] = buffer[start+1];
      lcu.l_alpha[3] = buffer[start+0];
   }  /* ends if lsb = 0      */

   *number = lcu.l_num;
}  /* ends extract_ulong_from_buffer */




   /****************************************
   *
   *   extract_short_from_buffer(...
   *
   *   This takes a two byte short out of a
   *   buffer of characters.
   *
   *   It is important to know the byte order
   *   LSB or MSB.
   *
   ****************************************/

extract_short_from_buffer(buffer, lsb, start, number)
   char  buffer[];
   int       lsb, start;
   short *number;
{

   int i;
   union short_char_union lcu;

   if(lsb == 1){
      lcu.s_alpha[0] = buffer[start+0];
      lcu.s_alpha[1] = buffer[start+1];
   }  /* ends if lsb = 1 */

   if(lsb == 0){
      lcu.s_alpha[0] = buffer[start+1];
      lcu.s_alpha[1] = buffer[start+0];
   }  /* ends if lsb = 0      */

   *number = lcu.s_num;


}  /* ends extract_short_from_buffer */





extract_ushort_from_buffer(buffer, lsb, start, number)
   char  buffer[];
   int       lsb, start;
   unsigned short *number;
{

   int i;
   union ushort_char_union lcu;

   if(lsb == 1){
      lcu.s_alpha[0] = buffer[start+0];
      lcu.s_alpha[1] = buffer[start+1];
   }  /* ends if lsb = 1 */

   if(lsb == 0){
      lcu.s_alpha[0] = buffer[start+1];
      lcu.s_alpha[1] = buffer[start+0];
   }  /* ends if lsb = 0      */

   *number = lcu.s_num;
}  /* ends extract_ushort_from_buffer */
