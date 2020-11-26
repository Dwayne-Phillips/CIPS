

   /************************************************
   *
   *       file tif2bmp.c
   *
   *       Functions: This file contains
   *           main
   *
   *       Purpose:
   *          This program creates a bmp file
   *          that is just like the input tiff file.
   *
   *       External Calls:
   *          imageio.c
   *             does_not_exist
   *             get_image_size
   *             read_image_array
   *             write_image_array
   *             free_image_array
   *             create_allocate_bmp_file
   *
   *       Modifications:
   *         27 September 1998 - created
   *
   *************************************************/

#include "cips.h"

int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();
int create_allocate_bmp_file();


int main(argc, argv)
   int  argc;
   char *argv[];
{
   char   *cc;
   int    l, w;
   int    ok = 0;
   short  **the_image;
   struct tiff_header_struct image_header;
   struct bmpfileheader      bmp_file_header;
   struct bitmapheader       bmheader;

   if(argc < 3 || argc > 3){
      printf(
      "\nusage: tif2bmp tif-file-name bmp-file-name\n");
      exit(-1);
   }

   if(does_not_exist(argv[1])){
    printf("\nERROR input file %s does not exist",
             argv[1]);
    exit(0);
   }

   cc = strstr(argv[1], ".tif");
   if(cc == NULL){
      printf("\nERROR %s must be a tiff file",
             argv[1]);
    exit(0);
   }  /* ends tif */

   cc = strstr(argv[2], ".bmp");
   if(cc == NULL){  /* create a bmp */
      printf("\nERROR %s must be a bmp file name",
             argv[2]);
      exit(0);
   }

   get_image_size(argv[1], &l, &w);
   the_image       = allocate_image_array(l, w);
   bmheader.height = l;
   bmheader.width  = w;
   create_allocate_bmp_file(argv[2],
                            &bmp_file_header,
                            &bmheader);

   read_image_array(argv[1], the_image);
   write_image_array(argv[2], the_image);
   free_image_array(the_image, l);

}  /* ends main */
