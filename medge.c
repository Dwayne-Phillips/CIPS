


   /***********************************************
   *
   *   file medge.c
   *
   *   Functions: This file contains
   *      main
   *
   *   Purpose:
   *      This file contains the main calling
   *      routine that performs edge 
   *      detection.
   *
   *   External Calls:
   *      imageio.c - create_image_file
   *                  read_image_array
   *                  write_image_array
   *                  get_image_size
   *                  allocate_image_array
   *                  free_image_array
   *      edge.c - 
   *          detect_edges
   *          setup_masks
   *          get_edge_options
   *          perform_convolution
   *          quick_edge
   *      edge2.c - 
   *          homogeneity
   *          difference_edge
   *          contrast_edge
   *          range
   *          variance
   *      edge3.c - 
   *          gaussian_edge
   *          enhance_edges
   *
   *   Modifications:
   *     18 September 1998 - created to work with 
   *           all I O routines in imageio.c.
   *
   *************************************************/



#include "cips.h"


int main(argc, argv)
   int argc;
   char *argv[];
{
   char  image_name[MAX_NAME_LENGTH];
   char  image_name2[MAX_NAME_LENGTH];
   char  response[MAX_NAME_LENGTH];
   int   i, j;
   int   high, size, threshold, type;
   long  bits_per_pixel, height, width;
   short **the_image, **out_image;
   struct   tiff_header_struct image_header;

   
      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc < 4   || argc > 7){
      show_edge_usage();
      exit(0);
   }

   strcpy(image_name,  argv[2]);
   strcpy(image_name2, argv[3]);

   if(does_not_exist(image_name)){
    printf("\nERROR input file %s does not exist",
             image_name);
    exit(0);
   }

   create_image_file(image_name, image_name2);
   get_image_size(image_name, &height, &width);
   get_bitsperpixel(image_name, &bits_per_pixel);
   the_image = allocate_image_array(height, width);
   out_image = allocate_image_array(height, width);
   read_image_array(image_name, the_image);

   if(argv[1][0] == 'q' || argv[1][0] == 'Q'){
      threshold = atoi(argv[4]);
      high      = atoi(argv[5]);
      quick_edge(the_image, out_image,
                 threshold, high,
                 height, width, 
                 bits_per_pixel);
   }  /* ends if q */

   if(argv[1][0] == 'b' || argv[1][0] == 'B'){
      threshold = atoi(argv[4]);
      high      = atoi(argv[5]);
      type      = atoi(argv[6]);
      perform_convolution(
                 the_image, out_image,
                 type, threshold,
                 height, width, 
                 bits_per_pixel, high);
   }  /* ends if b */

   if(argv[1][0] == 'h' || argv[1][0] == 'H'){
      threshold = atoi(argv[4]);
      high      = atoi(argv[5]);
      homogeneity(the_image, out_image,
                  height, width, 
                  bits_per_pixel,
                  threshold, high);
   }  /* ends if h */

   if(argv[1][0] == 'd' || argv[1][0] == 'D'){
      threshold = atoi(argv[4]);
      high      = atoi(argv[5]);
      difference_edge(the_image, out_image,
                      height, width, 
                      bits_per_pixel,
                      threshold, high);
   }  /* ends if d */

   if(argv[1][0] == 'c' || argv[1][0] == 'C'){
      threshold = atoi(argv[4]);
      high      = atoi(argv[5]);
      contrast_edge(the_image, out_image,
                    height, width, 
                    bits_per_pixel,
                    threshold, high);
   }  /* ends if c */

   if(argv[1][0] == 'r' || argv[1][0] == 'R'){
      threshold = atoi(argv[4]);
      high      = atoi(argv[5]);
      size      = atoi(argv[6]);
      range(the_image, out_image,
            height, width, 
            bits_per_pixel,
            size, threshold, high);
   }  /* ends if r */

   if(argv[1][0] == 'v' || argv[1][0] == 'V'){
      threshold = atoi(argv[4]);
      high      = atoi(argv[5]);
      variance(the_image, out_image,
               height, width, 
               bits_per_pixel,
               threshold, high);
   }  /* ends if v */

   if(argv[1][0] == 'g' || argv[1][0] == 'G'){
      threshold = atoi(argv[4]);
      high      = atoi(argv[5]);
      size      = atoi(argv[6]);
      gaussian_edge(the_image, out_image,
               height, width, 
               bits_per_pixel,
               size, threshold, high);
   }  /* ends if g */

   if(argv[1][0] == 'e' || argv[1][0] == 'E'){
      high      = atoi(argv[4]);
      enhance_edges(the_image, out_image,
               height, width, 
               bits_per_pixel, high);
   }  /* ends if q */

   write_image_array(image_name2, out_image);
   free_image_array(the_image, height);
   free_image_array(out_image, height);

}  /* ends main */


show_edge_usage()
{
printf("\nusage of medge"
"\n  Quick edge detector"
"\nmedge Q in-file out-file threshold (1/0) high"
"\n  Sobel Kirsch Prewitt edge detectors"
"\nmedge B in-file out-file threshold (1/0) high type (1,2,3)"
"\n  Homogeneity edge detector"
"\nmedge H in-file out-file threshold (1/0) high"
"\n  Difference edge detector"
"\nmedge D in-file out-file threshold (1/0) high"
"\n  Contrast edge detector"
"\nmedge C in-file out-file threshold (1/0) high"
"\n  Range edge detector"
"\nmedge R in-file out-file threshold (1/0) high size(3,5,7...)"
"\n  Variance edge detector"
"\nmedge V in-file out-file threshold (1/0) high"
"\n  Guassian edge detector"
"\nmedge G in-file out-file threshold (1/0) high size(7 or 9)"
"\n  Enhance edges"
"\nmedge E in-file out-file high "
"\n");
}  /* ends show_edge_usage */
