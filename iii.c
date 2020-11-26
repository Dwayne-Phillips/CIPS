
   /***********************************************
   *
   *   file iii.c
   *
   *   Functions: This file contains
   *      main
   *
   *   Purpose:
   *      This is a test program.
   *
   *   External Calls:
   *          imageio.c
   *             create_allocate_tif_file
   *             create_allocate_bmp_file
   *             get_image_size
   *             allocate_image_array
   *             free_image_array
   *             read_image_array
   *             write_image_array
   *
   *   Modifications:
   *      18 February 2016 - created
   *
   ***********************************************/

#include "cips.h"

int grow();
int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();
int create_allocate_tiff_file();
int create_allocate_bmp_file();


int main(argc, argv)
   int argc;
   char *argv[];
{
   char     name1[80], name2[80];
   char     *cc;
   int      a, b;
   int      ok = 0;
   long     length, width;
   short    **the_image;
   struct tiff_header_struct image_header;
   struct bmpfileheader      bmp_file_header;
   struct bitmapheader       bmheader;

   if(argc != 3){
      printf("\nusage: III-TEST-PROGRAM in-file out-file\n");
      exit(1);
   }

   strcpy(name1, argv[1]);
   strcpy(name2, argv[2]);

   get_image_size(name1, &length, &width);

   the_image = allocate_image_array(length, width);

   cc = strstr(name1, ".tif");
   if(cc != NULL){  /* create a tif */
      ok = 1;
      image_header.lsb            = 1;
      image_header.bits_per_pixel = 8;
      image_header.image_length   = length;
      image_header.image_width    = width;
      image_header.strip_offset   = 1000;
      create_allocate_tiff_file(name2,
                                &image_header);
   }  /* ends tif */

   cc = strstr(argv[1], ".bmp");
   if(cc != NULL){  /* create a bmp */
      ok = 1;
      bmheader.height = length;
      bmheader.width  = width;
      create_allocate_bmp_file(argv[2],
                               &bmp_file_header,
                               &bmheader);
   }  /* ends tif */

   if(ok == 0){
      printf("\nERROR input file neither tiff nor bmp");
      exit(0);
   }

   read_image_array(name1, the_image);

/************
   for(a=0; a<length; a++){
      for(b=0; b<width; b++){
         the_image[a][b] = 0;
      }
   }

   for(a=0; a<length; a++){
      for(b=0; b<width; b++){
         if(a % 50 == 0 && b % 50 == 0)
             the_image[a][b] = 100;
      }
   }

   for(a=0; a<length; a++){
      for(b=0; b<width; b++){
         the_image[a][b] = 0;
      }
   }

   for(a=10; a<50; a++)
      for(b=10; b<50; b++)
         the_image[a][b] = 100;

   for(a=110; a<150; a++)
      for(b=110; b<150; b++)
         the_image[a][b] = 100;

   for(a=210; a<250; a++)
      for(b=210; b<250; b++)
         the_image[a][b] = 100;

   for(a=310; a<350; a++)
      for(b=310; b<350; b++)
         the_image[a][b] = 100;

   for(a=10; a<50; a++)
      for(b=60; b<110; b++)
         the_image[a][b] = 100;

   for(a=10; a<50; a++)
      for(b=120; b<170; b++)
         the_image[a][b] = 100;

   for(a=10; a<50; a++)
      for(b=180; b<230; b++)
         the_image[a][b] = 100;

   for(a=10; a<50; a++)
      for(b=240; b<290; b++)
         the_image[a][b] = 100;

   for(a=10; a<50; a++)
      for(b=300; b<350; b++)
         the_image[a][b] = 100;

   for(a=360; a<365; a++)
      for(b=360; b<365; b++)
         the_image[a][b] = 100;

   for(a=370; a<375; a++)
      for(b=370; b<375; b++)
         the_image[a][b] = 100;

   for(a=380; a<385; a++)
      for(b=380; b<385; b++)
         the_image[a][b] = 100;


   the_image[390][390] = 100;
   the_image[392][392] = 100;
   the_image[394][394] = 100;
   the_image[396][396] = 100;
***************/


   grow(the_image, 100, length, width);
   write_image_array(name2, the_image);
   free_image_array(the_image, length);

}  /* ends main */
