
    /***********************************************
    *
    *   file mainseg.c
    *
    *   Functions: This file contains
    *      main
    *
    *   Purpose:
    *      This file contains the main calling
    *      routine in a segmentation and related
    *      operations program.
    *
    *   External Calls:
    *      imageio.c - create_image_file
    *                  read_image_array
    *                  write_image_array
    *                  get_image_size
    *                  allocate_image_array
    *                  free_image_array
    *      segment.c - threshold_image_array
    *                  grow
    *                  find_peaks
    *                  peaks_high_low
    *                  valley_high_low
    *                  threshold_and_find_means
    *
    *   Modifications:
    *      27 September 1992 - created
    *      15 August 1998 - modified to work on entire
    *           images at once.
    *      19 September 1998 - modified to work with
    *            all I O routines in imageio.c.
    *
    *************************************************/

#include "cips.h"

int does_not_exist();
int get_image_size();
int create_file_if_needed();
int read_image_array();
int free_image_array();
int write_image_array();
int manual_threshold_segmentation();
int grow();
int peak_threshold_segmentation();
int valley_threshold_segmentation();
int adaptive_threshold_segmentation();


int main(argc, argv)
   int argc;
   char *argv[];
{
   char   name1[80], name2[80], response[80];
   int    segment;
   long   length, width;
   short  background, hi, low, object, value;
   short  **the_image, **out_image;

      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc < 8){
    printf(
     "\n\nmainseg in-file out-file hi low "
     "value operation segment"
     "\n"
     "\n\thi and low are thresholds"
     "\n\tvalue is output pixel value"
     "\n\toperation = Threshold Grow Peaks"
     " Valleys Adaptive"
     "\n\tsegment is 1 (perform segmentation) "
     "or 0 (don't)");
    printf("\n");
    exit(0);
   }

   strcpy(name1, argv[1]);
   strcpy(name2, argv[2]);
   hi      = atoi(argv[3]);
   low     = atoi(argv[4]);
   value   = atoi(argv[5]);
   segment = atoi(argv[7]);

   if(does_not_exist(name1)){
    printf("\nERROR input file %s does not exist",
             name1);
    exit(0);
   }

      /******************************************
      *
      *   Read the input image header, allocate
      *   the image arrays, create the output
      *   image, and read the input image.
      *
      ******************************************/


   get_image_size(name1, &length, &width);

   the_image = allocate_image_array(length, width);
   out_image = allocate_image_array(length, width);

   create_file_if_needed(name1, name2, out_image);
   read_image_array(name1, the_image);

      /*********************************
      *
      *   Manual Threshold operation
      *
      *********************************/

  if(argv[6][0] == 't' || argv[6][0] == 'T'){
     manual_threshold_segmentation(
        the_image, out_image,
        hi, low, value, segment,
        length, width);
     write_image_array(name2, out_image);
  }  /* ends if t */


      /*********************************
      *
      *   Grow region operation
      *
      *********************************/

  if(argv[6][0] == 'g' || argv[6][0] == 'G'){
     grow(the_image, value,
          length, width);
     write_image_array(name2, the_image);
  }  /* ends if g */


      /*********************************
      *
      *   Peak threshold operation
      *
      *********************************/

   if(argv[6][0] == 'p' || argv[6][0] == 'P'){
      peak_threshold_segmentation(
         the_image, out_image,
         value, segment,
         length, width);
     write_image_array(name2, out_image);
   }  /* ends if p */



      /*********************************
      *
      *   Valley threshold operation
      *
      *********************************/

   if(argv[6][0] == 'v' || argv[6][0] == 'V'){
      valley_threshold_segmentation(
         the_image, out_image,
         value, segment,
         length, width);
     write_image_array(name2, out_image);
   }  /* ends if v */

      /*********************************
      *
      *   Adaptive threshold operation
      *
      *********************************/

   if(argv[6][0] == 'a' || argv[6][0] == 'A'){
      adaptive_threshold_segmentation(
         the_image, out_image,
         value, segment,
         length, width);
     write_image_array(name2, out_image);
   }  /* ends if a */

   free_image_array(out_image, length);
   free_image_array(the_image, length);

}  /* ends main */
