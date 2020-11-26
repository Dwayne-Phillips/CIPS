


   /***********************************************
   *
   *   file testbed.c
   *
   *   Functions: This file contains
   *      main
   *
   *   Purpose:
   *      This file contains the main calling
   *      routine that performs histogram
   *      equalization.
   *
   *   External Calls:
   *      imageio.c - create_image_file
   *                  read_image_array
   *                  write_image_array
   *                  get_image_size
   *                  allocate_image_array
   *                  free_image_array
   *      hist.c - calculate_histogram
   *               perform_histogram_equalization
   *
   *   Modifications:
   *     18 September 1998 - created to work with
   *           all I O routines in imageio.c.
   *
   *************************************************/

#include "cips.h"

int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();
int calculate_histogram();
int perform_histogram_equalization();


/*********
short** allocate_image_array(long, long);
int free_image_array(short **, long);
*********/


short** allocate_xxx_array(length, width)
   long  length, width;
{
   int i, j;

   short** the_array = (short**) malloc(length * sizeof(short*));
   for(i=0; i<length; i++){
      the_array[i] = (short*) malloc(width * sizeof(short));
      if(the_array[i] == NULL){
         printf("\n\tmalloc of the_image[%d] failed", i);
      }  /* ends if */
   }  /* ends loop over i */

   for(i=0; i<length; i++)
      for(j=0; j<width; j++)
         the_array[i][j] = i+j;

   return(the_array);

}  /* ends allocate_image_array */

int free_xxx_array(the_array, length)
   short **the_array;
   long  length;
{
   int i;
   for(i=0; i<length; i++)
      free(the_array[i]);
   free(the_array);  /***** HERE ****/
   return(1);
}  /* ends free_image_array */




/***************************************************/
/***************************************************/
/***************************************************/


int main(argc, argv)
   int argc;
   char *argv[];
{
   char  in_name[MAX_NAME_LENGTH];
   char  out_name[MAX_NAME_LENGTH];
   char  response[MAX_NAME_LENGTH];
   int   i, j;
   long  height, width;
   short **the_image;
   unsigned long histogram[GRAY_LEVELS+1];
   struct tiff_header_struct tiff_file_header;


      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/


   if(argc < 3){
    printf("\n   usage: testbed input-image output-image");
    printf("\n\n");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   strcpy(out_name, argv[2]);

   if(does_not_exist(in_name)){
      printf("\nERROR input file %s does not exist",
             in_name);
      printf("\n      "
             "usage: testbed input-image output-image");
      exit(0);
   }

   get_image_size(in_name, &height, &width);
   the_image = allocate_image_array(height, width);
   for(j=0; j<10; j++)
      printf("-%3d-", the_image[5][j]);
   read_image_array(in_name, the_image);
   for(j=0; j<10; j++)
      printf("-%3d-", the_image[5][j]);
   create_image_file(in_name, out_name);
   printf("\nTB> height=%ld width=%ld\n", height, width);

   for(i=0; i<GRAY_LEVELS+1; i++) histogram[i] = 0;

   calculate_histogram(the_image, histogram,
                       height, width);

   perform_histogram_equalization(
                       the_image, histogram,
                       256, 250,
                       height, width);

   write_image_array(out_name, the_image);
   free_image_array(the_image, height);

/*************
   the_image = allocate_image_array(300, 300);
   for(j=0; j<10; j++)
      printf("-%3d-", the_image[5][j]);
   read_image_array(in_name, the_image);
   for(j=0; j<10; j++)
      printf("-%3d-", the_image[5][j]);
   free_image_array(the_image, height);
*****************/


/*******************
   create_image_file(in_name, out_name);
   get_image_size(in_name, &height, &width);
   printf("\nTB> height=%d width=%d\n", height, width);
   the_image = allocate_image_array(height, width);
   read_image_array(in_name, the_image);

for(j=0; j<10; j++)
printf("-%3d-", the_image[5][j]);

   for(i=0; i<GRAY_LEVELS+1; i++) histogram[i] = 0;

   calculate_histogram(the_image, histogram,
                       height, width);
printf("\nTB> Calculated the histogram");
   perform_histogram_equalization(
                       the_image, histogram,
                       256, 250,
                       height, width);
printf("\nTB> Equalized the histogram");

   write_image_array(out_name, the_image);
printf("\nTB> wrote output image");
   free_image_array(the_image, height);
******************/

/***********
   if(is_a_tiff(in_name)){
printf("\nTB> inside if is a tiff\n");
      read_tiff_header(in_name, &tiff_file_header);
printf("\nTB> Read the tiff header\n");
      create_allocate_tiff_file(out_name,
                                &tiff_file_header);
printf("\nCIF> created allocated tiff file\n");
   }


      read_tiff_header(in_name, &tiff_file_header);
printf("\nTB> Read the tiff header\n");
      create_allocate_tiff_file(out_name,
                                &tiff_file_header);
printf("\nCIF> created allocated tiff file\n");
   }
***********/

/****************
   create_image_file(in_name, out_name);
   get_image_size(in_name, &height, &width);
   the_image = allocate_image_array(height, width);
   read_image_array(in_name, the_image);

   for(i=0; i<GRAY_LEVELS+1; i++) histogram[i] = 0;

   calculate_histogram(the_image, histogram,
                       height, width);

   perform_histogram_equalization(
                       the_image, histogram,
                       256, 250,
                       height, width);

   write_image_array(out_name, the_image);
   free_image_array(the_image, height);
******************/

}  /* ends main */
