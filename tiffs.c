
   /***************************
   *
   *   tiffs.c
   *   COMPOSITE FILE COMPRISING:
   *   rtiff.c
   *   wtiff.c
   *   tiff.c
   *
   *   November 2020
   *   This is sort of a mess.
   *   Much of what was in this file
   *   is now in imageio.c
   *   I cannot find the original wtiff rtiff etc.
   *
   ***************************/



       /*********************************************
       *
       *  file rtiff.c
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


int does_not_exist();
int read_line();
int read_tiff_header();
int round_off_image_size();
int create_allocate_tiff_file();
int insert_short_into_buffer();
int insert_long_into_buffer();
int seek_to_first_line();
int seek_to_end_of_line();
int write_line();
int extract_long_from_buffer();
int extract_short_from_buffer();
int extract_ulong_from_buffer();
int extract_ushort_from_buffer();



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





       /*********************************************
       *
       * write_array_into_tiff_file(...
       *
       * This function takes an array of shorts and
       * writes them into an existing tiff image file.
       *
       **********************************************/

int write_array_into_tiff_image(image_file_name, array,
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
return(1);
}  /*  ends write_array_into_tiff_image */




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
