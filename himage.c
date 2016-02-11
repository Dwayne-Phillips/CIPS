

   /************************************************
   *
   *       file himage.c
   *
   *       Functions: This file contains
   *           main
   *           vline
   *           hline
   *
   *       Purpose:
   *          This program calculates the histogram
   *          of an image and puts the picture of
   *          that histogram in an output image.
   *
   *       External Calls:
   *          imageio.c
   *             does_not_exist
   *             create_allocate_tiff_file
   *             create_allocate_bmp_file
   *             get_image_size
   *             allocate_image_array
   *             free_image_array
   *             read_image_array
   *             write_image_array
   *          hist.c
   *             calculate_histogram
   *
   *       Modifications:
   *          7 Arpil 1992 - created
   *         15 August 1998 - modified to work with
   *              an entire image array at once.
   *         22 September 1998 - modified to work with 
   *              all I O routines in imageio.c.
   *
   *************************************************/

#include "cips.h"

#define W    300
#define L    300
#define UP     5
#define LEFT   5
#define SPOT 200


main(argc, argv)
   int  argc;
   char *argv[];
{
   char   *cc;
   int    i, j, amount;
   long   l, w;
   int    ok = 0;
   long   length, width;
   short  **image, **hist;
   struct tiff_header_struct image_header;
   struct bmpfileheader      bmp_file_header;
   struct bitmapheader       bmheader;
   unsigned long histogram[GRAY_LEVELS+1];
   int     count;
   unsigned long max, scale;

   max   = 0;
   count = 0;

   if(argc < 3 ){
      printf(
      "\nusage: himage image-file histogram-file [length width]\n");
      exit(-1);
   }

   if(does_not_exist(argv[1])){
    printf("\nERROR input file %s does not exist",
             argv[1]);
    exit(0);
   }

   if(argc >= 4)
      l = atoi(argv[3]);
   else
      l = L;

   if(argc >= 5)
      w = atoi(argv[4]);
   else
      w = W;

   cc = strstr(argv[2], ".tif");
   if(cc != NULL){  /* create a tif */
      ok = 1;
      image_header.lsb            = 1;
      image_header.bits_per_pixel = 8;
      image_header.image_length   = l;
      image_header.image_width    = w;;
      image_header.strip_offset   = 1000;
      create_allocate_tiff_file(argv[2], 
                                &image_header);
   }  /* ends tif */

   cc = strstr(argv[2], ".bmp");
   if(cc != NULL){  /* create a bmp */
      ok = 1;
      bmheader.height = l;
      bmheader.width  = w;
      create_allocate_bmp_file(argv[2], 
                               &bmp_file_header, 
                               &bmheader);
   }  /* ends tif */

   if(ok == 0){
      printf("\nERROR input file neither tiff nor bmp");
      exit(0);
   }

   get_image_size(argv[1], &length, &width);
   image = allocate_image_array(length, width);
   hist  = allocate_image_array(l, w);
   read_image_array(argv[1], image);

   for(i=0; i<l; i++)
      for(j=0; j<w; j++)
         hist[i][j] = 0;

   for(i=0; i<GRAY_LEVELS+1; i++) histogram[i] = 0;

   calculate_histogram(image, histogram, 
                       length, width);


   hline(hist, l-UP, LEFT, LEFT+GRAY_LEVELS+1);
   vline(hist, LEFT+ 50, l-UP+2, l-UP);
   vline(hist, LEFT+100, l-UP+2, l-UP);
   vline(hist, LEFT+150, l-UP+2, l-UP);
   vline(hist, LEFT+200, l-UP+2, l-UP);
   vline(hist, LEFT+250, l-UP+2, l-UP);

   for(i=0; i<GRAY_LEVELS+1; i++)
      if(histogram[i] > max)
         max = histogram[i];

   if(max > (l-UP-UP))
      scale = max/(l-5*UP);
   else
      scale = 1;

   printf("\n max=%ld scale=%ld",max, scale);

   for(i=0; i<GRAY_LEVELS+1; i++){
      amount = histogram[i]/scale;
      if(amount > 0){
         vline(hist, i+LEFT, l-UP, l-UP-amount);
      }  /* ends if not zero */
   }  /* ends loop over i GRAY_LEVELS */

   write_image_array(argv[2], hist);

   free_image_array(image, length);
   free_image_array(hist, l);

}  /* ends main */





vline(image, ie, il, ll)
   int ie, il, ll;
   short **image;
{
   int i, j;

   for(i=il; i>=ll; i--)
      image[i][ie] = SPOT;

}  /* ends vline */




hline(image, il, ie, le)
   int il, ie, le;
   short **image;
{
   int i, j;

   for(i=ie; i<=le; i++)
      image[il][i] = SPOT;

}  /* ends hline */
