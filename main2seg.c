
       /***********************************************
       *
       *   file main2seg.c
       *
       *   Functions: This file contains
       *      main
       *
       *   Purpose:
       *      This is a calling program that calls
       *      the three new segmentation techniques
       *      discussed in Image Processing part 10.
       *
       *   External Calls:
       *      imageio.c - does_not_exist
       *                  get_image_size
       *                  get_bitsperpixel
       *                  allocate_image_array
       *                  free_image_array
       *                  create_file_if_needed
       *                  read_image_array
       *                  write_image_array
       *      segment2.c - edge_region
       *                   gray_shade_region
       *                   edge_gray_shade_region
       *
       *   Modifications:
       *      5 December 1992 - created
       *      15 August 1998 - modified to work on entire
       *           images at once.
       *
       ***********************************************/

#include "cips.h"



short **the_image;
short **out_image;

main(argc, argv)
   int argc;
   char *argv[];
{

   char     name1[80], name2[80], low_high[80], type[80];
   float    percent;
   int      i, j, 
            looking = 1;
   long     length, width, bits_per_pixel;
   short    value,  value2, value3, 
            value4, value5, value6;
   struct   tiff_header_struct image_header;

       /***********************************************
       *
       *       Interpret the command line parameters.
       *
       ************************************************/

   if(argc < 4){
    printf(
    "\n\nNot enough parameters:"
     "\n"
     "\n usage: main2seg in-file out-file type"
     " [values ...]"
     "\n"
     "\n   recall type: Edge-region edge-gray-grow (C)"
     " Gray-shade-grow"
     "\n"
    "\n   main2seg in-file out-file C percent "
    "edge-type "
    "\n            min-area max-area diff set-value erode"
    "\n   main2seg in-file out-file E percent "
    "edge-type "
    "\n            min-area max-area diff set-value erode"
    "\n   main2seg in-file out-file G diff "
    "min-area max-area"
    "\n"
    "\n");
    exit(0);
   }

   strcpy(name1,  argv[1]);
   strcpy(name2, argv[2]);
   strcpy(type,  argv[3]);
   if(type[0] == 'e' || type[0] == 'E'  ||
      type[0] == 'c' || type[0] == 'C'){
      percent = atof(argv[4]);
      value   = atoi(argv[5]);
      value2  = atoi(argv[6]);
      value3  = atoi(argv[7]);
      value4  = atoi(argv[8]);
      value5  = atoi(argv[9]);
      value6  = atoi(argv[10]);
    }
   else{
      value  = atoi(argv[4]);
      value2 = atoi(argv[5]);
      value3 = atoi(argv[6]);
   }

   if(does_not_exist(name1)){
    printf("\nERROR input file %s does not exist",
             name1);
    exit(0);
   }


       /******************************************
       *
       *  Read the input image header, allocate 
       *  the image arrays, and read the input
       *  image.
       *
       *******************************************/

   get_image_size(name1, &length, &width);
   get_bitsperpixel(name1, &bits_per_pixel);
  
   the_image = allocate_image_array(length, width);
   out_image = allocate_image_array(length, width);

   create_file_if_needed(name1, name2, out_image);
   read_image_array(name1, the_image);


   if(type[0] == 'e'  || type[0] == 'E'){
      edge_region(the_image, out_image, 
                  value, value2, 
                  value3, value4, percent, 
                  value5, value6,
                  length,
                  width,
                  bits_per_pixel);
   }  /* ends edge_region */



   if(type[0] == 'g'  || type[0] == 'G'){
      gray_shade_region(the_image, out_image, 
                        value,
                        value2, value3,
                        length,
                        width);
   }  /* ends gray_shade_region */



   if(type[0] == 'c'  || type[0] == 'C'){
      edge_gray_shade_region(name1,
               the_image, out_image, 
               value, value2, value3, value4, 
               percent, value5, value6,
               length,
               width,
               bits_per_pixel);
   }  /* ends edge_gray_shade_region */

   write_image_array(name2, out_image);

   free_image_array(out_image, length);
   free_image_array(the_image, length);

}  /* ends main  */
