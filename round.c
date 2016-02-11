
       /***********************************************
       *
       *       file c:\cips\round.c
       *
       *       Functions: This file contains
       *          main
       *
       *       Purpose:
       *          This program takes an image file and
       *          rounds if off by copying a part of it
       *          to another file.
       *
       *       External Calls:
       *         imageio.c - does_not_exit
       *                     get_image_size 
       *                     allocate_image_array 
       *                     read_image_array 
       *                     is_a_tiff 
       *                     is_a_bmp
       *                     create_allocate_tiff_file
       *                     read_bmp_file_header
       *                     read_bm_header
       *                     create_allocate_bmp_file
       *                     write_image_array
       *                     free_image_array
       *                
       *
       *       Modifications:
       *          31 March 1991 - created
       *          8  May 1993 - Made this program
       *                        command line driven.
       *          6 August 1998 - Made this work with
       *              entire image arrays at once.
       *         18 September 1998 - modified to work with 
       *               all I O routines in imageio.c.
       *
       ***********************************************/

#include "cips.h"


main(argc, argv)
   int  argc;
   char *argv[];
{
char response[80];
   char     name[80], name2[80];
   int      i          = 0, 
            ie         = 0, 
            il         = 0, 
            j          = 0, 
            in_length  = 0, 
            out_length = 0,
            in_width   = 0,
            out_width  = 0;
   short    **the_image, **out_image;
   
   struct bmpfileheader      bmp_file_header;
   struct bitmapheader       bmheader;
   struct tiff_header_struct tiff_file_header;

      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc < 5   ||
     (argc > 5  &&  argc < 7)){
      printf("\nusage: roundoff in-image out-image"
          " length width [il ie]"
          "\n"
          "\n       If you do not specify il ie"
          " they will be set to 1 1."
          "\n       ll le will always be"
          " il+length and ie+width"
          "\n");
      exit(0);
   }

   strcpy(name,  argv[1]);
   strcpy(name2, argv[2]);
   out_length = atoi(argv[3]);
   out_width  = atoi(argv[4]);

   if(argc > 5){
      il = atoi(argv[5]);
      ie = atoi(argv[6]);
   }

   if(does_not_exist(name)){
    printf("\nERROR input file %s does not exist",
             name);
    exit(0);
   }

   get_image_size(name, &in_length, &in_width);
   the_image = allocate_image_array(in_length,
                                    in_width);
   read_image_array(name, the_image);
   
      /******************************************
      *
      *   Create the output image and allocate
      *   the output image array.
      *
      ******************************************/

   if(is_a_tiff(name)){
      read_tiff_header(name, &tiff_file_header);
      tiff_file_header.image_length = out_length;
      tiff_file_header.image_width  = out_width;
      create_allocate_tiff_file(name2,
                                &tiff_file_header);
   }

   if(is_a_bmp(name)){
      read_bmp_file_header(name, 
                           &bmp_file_header);
      read_bm_header(name, &bmheader);
      bmheader.height = out_length;
      bmheader.width  = out_width;
      create_allocate_bmp_file(name2, 
                               &bmp_file_header, 
                               &bmheader);
   }

   out_image = allocate_image_array(out_length, out_width);

      /******************************************
      *
      *   Copy the input image array to the output
      *   image array per the input parameters.
      *
      ******************************************/

   for(i=0; i<out_length; i++)
      for(j=0; j<out_width; j++)
         out_image[i][j] = the_image[i+il][j+ie];

   write_image_array(name2, out_image);

   free_image_array(out_image, out_length);
   free_image_array(the_image, in_length);

}  /* ends main */
