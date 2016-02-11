

#include "cips.h"





main(argc, argv)
   int argc;
   char *argv[];
{

   struct bmpfileheader file_header;
   struct bitmapheader  bmheader;
   struct ctstruct rgb[256];

   FILE *fp;
   short **the_image;
   int   i, j;


   long height = 0, width = 0;
   long bitsperpixel = 0;
   int  negative = 0;

      /******************************************
      *
      *
      ******************************************/

   if(argc < 2){
      printf("\ntestbmp file-name.bmp");
      exit(0);
   }


   if(get_image_size(argv[1], &height, &width)){
      printf("\nSize of %s rows=%ld cols=%ld",argv[1],width,height);
      the_image = allocate_image_array(height, width);
      read_image_array(argv[1], the_image);
      write_image_array(argv[2], the_image);
      free_image_array(the_image, height);
   }



#ifdef TESTREAD
   if(is_a_tiff(argv[1]))
      printf("\n%s is a tif", argv[1]);
   else
      printf("\n%s is not a tif", argv[1]);

   if(get_image_size(argv[1], &height, &width)){
      printf("\nSize of %s rows=%ld cols=%ld",argv[1],width,height);
      the_image = allocate_image_array(height, width);
      read_image_array(argv[1], the_image);
      free_image_array(the_image, height);
   }
#endif


#ifdef TESTBITS
   if(get_bitsperpixel(argv[1], &bitsperpixel))
      printf("\nBits per pixel of %s bits=%ld",argv[1],bitsperpixel);
   else
      printf("\nCould not get the size of %s", argv[1]);
#endif


#ifdef TESTSIZE
   if(get_image_size(argv[1], &height, &width))
      printf("\nSize of %s rows=%ld cols=%ld",argv[1],width,height);
   else
      printf("\nCould not get the size of %s", argv[1]);
#endif


#ifdef NEVER
   read_bmp_file_header(argv[1], &file_header);
   print_bmp_file_header(&file_header);

   read_bm_header(argv[1], &bmheader);
   print_bm_header(&bmheader);

   width = bmheader.width;
   if(bmheader.height < 0)
      height   = bmheader.height * (-1);
   else
      height = bmheader.height;

   the_image = allocate_image_array(height, width);
   read_bmp_image(argv[1], the_image);

   for(i=0; i<15; i++){
      printf("\n");
      for(j=0; j<15; j++){
         printf("-%3d", the_image[i][j]);
      }
   }

   free_image_array(the_image, height);
#endif



#ifdef TESTWRITE
   read_bm_header(argv[1], &bmheader);

   width = bmheader.width;
   if(bmheader.height < 0)
      height   = bmheader.height * (-1);
   else
      height = bmheader.height;

   the_image = allocate_image_array(height, width);

   for(i=0; i<height; i++){
      for(j=0; j<width; j++){
         the_image[i][j] = i;
      }
   }

   write_bmp_image(argv[1], the_image);
   free_image_array(the_image, height);
#endif



#ifdef IFNEEDED
   read_bm_header(argv[1], &bmheader);

   width = bmheader.width;
   if(bmheader.height < 0)
      height   = bmheader.height * (-1);
   else
      height = bmheader.height;

   the_image = allocate_image_array(height, width);

   create_bmp_file_if_needed(argv[1], argv[2], the_image);

   free_image_array(the_image, height);
#endif




#ifdef TESTCREATE
   bmheader.height = 222;
   bmheader.width  = 333;
   create_allocate_bmp_file(argv[1], &file_header, &bmheader);
#endif


#ifdef TESTREAD
   read_bmp_file_header(argv[1], &file_header);
   print_bmp_file_header(&file_header);

   read_bm_header(argv[1], &bmheader);
   print_bm_header(&bmheader);

   width = bmheader.width;
   if(bmheader.height < 0)
      height   = bmheader.height * (-1);
   else
      height = bmheader.height;

   the_image = allocate_image_array(height, width);
   read_bmp_image(argv[1], the_image);

   for(i=0; i<15; i++){
      printf("\n");
      for(j=0; j<15; j++){
         printf("-%3d", the_image[i][j]);
      }
   }

   free_image_array(the_image, height);
#endif

      /******************************************
      *
      *
      *
      ******************************************/

}  /* ends main */






      /******************************************
      *
      *   read_bmp_file_header(...
      *
      *   This function reads the bmpfileheader
      *   structure from the top of a bmp
      *   image file.
      *
      ******************************************/

read_bmp_file_header(file_name,
                     file_header)
   char *file_name;
   struct bmpfileheader *file_header;
{
   char  buffer[10];
   long  ll;
   short ss;
   unsigned long  ull;
   unsigned short uss;
   FILE     *fp;

   fp = fopen(file_name, "rb");

   fread(buffer, 1, 2, fp);
   extract_ushort_from_buffer(buffer, 1, 0, &uss);
   file_header->filetype = uss;

   fread(buffer, 1, 4, fp);
   extract_ulong_from_buffer(buffer, 1, 0, &ull);
   file_header->filesize = ull;

   fread(buffer, 1, 2, fp);
   extract_short_from_buffer(buffer, 1, 0, &ss);
   file_header->reserved1 = ss;

   fread(buffer, 1, 2, fp);
   extract_short_from_buffer(buffer, 1, 0, &ss);
   file_header->reserved2 = ss;

   fread(buffer, 1, 4, fp);
   extract_ulong_from_buffer(buffer, 1, 0, &ull);
   file_header->bitmapoffset = ull;

   fclose(fp);

}  /* ends read_bmp_file_header */



      /******************************************
      *
      *   print_bmp_file_header(...
      *
      *   This function printfs parts of the
      *   bmpfileheader structure.
      *
      ******************************************/

print_bmp_file_header(struct bmpfileheader *file_header)
{
 printf("\nfile type %x", file_header->filetype);
 printf("\nfile size %d", file_header->filesize);
 printf("\nbit map offset %d", file_header->bitmapoffset);
}




      /******************************************
      *
      *   read_bm_header(...
      *
      *   This function reads the bitmapheader
      *   structure from the top of a bmp
      *   image file.
      *
      ******************************************/

read_bm_header(file_name,
               bmheader)
   char *file_name;
   struct bitmapheader *bmheader;
{
   char  buffer[10];
   long  ll;
   short ss;
   unsigned long  ull;
   unsigned short uss;
   FILE *fp;

   fp = fopen(file_name, "rb");

      /****************************************
      *
      *   Seek past the first 14 byte header.
      *
      ****************************************/

   fseek(fp, 14, SEEK_SET);

   fread(buffer, 1, 4, fp);
   extract_ulong_from_buffer(buffer, 1, 0, &ull);
   bmheader->size = ull;

   fread(buffer, 1, 4, fp);
   extract_long_from_buffer(buffer, 1, 0, &ll);
   bmheader->width = ll;

   fread(buffer, 1, 4, fp);
   extract_long_from_buffer(buffer, 1, 0, &ll);
   bmheader->height = ll;

   fread(buffer, 1, 2, fp);
   extract_ushort_from_buffer(buffer, 1, 0, &uss);
   bmheader->planes = uss;

   fread(buffer, 1, 2, fp);
   extract_ushort_from_buffer(buffer, 1, 0, &uss);
   bmheader->bitsperpixel = uss;

   fread(buffer, 1, 4, fp);
   extract_ulong_from_buffer(buffer, 1, 0, &ull);
   bmheader->compression = ull;

   fread(buffer, 1, 4, fp);
   extract_ulong_from_buffer(buffer, 1, 0, &ull);
   bmheader->sizeofbitmap = ull;

   fread(buffer, 1, 4, fp);
   extract_ulong_from_buffer(buffer, 1, 0, &ull);
   bmheader->horzres = ull;

   fread(buffer, 1, 4, fp);
   extract_ulong_from_buffer(buffer, 1, 0, &ull);
   bmheader->vertres = ull;

   fread(buffer, 1, 4, fp);
   extract_ulong_from_buffer(buffer, 1, 0, &ull);
   bmheader->colorsused = ull;

   fread(buffer, 1, 4, fp);
   extract_ulong_from_buffer(buffer, 1, 0, &ull);
   bmheader->colorsimp = ull;

   fclose(fp);

}  /* ends read_bm_header */



      /******************************************
      *
      *   print_bm_header(...
      *
      *   This function printfs parts of the
      *   bitmapheader structure.
      *
      ******************************************/

print_bm_header(bmheader)
   struct bitmapheader *bmheader;
{
 printf("\nwidth %d", bmheader->width);
 printf("\nheight %d", bmheader->height);
 printf("\nplanes %d", bmheader->planes);
 printf("\nbitsperpixel %d", bmheader->bitsperpixel);
 printf("\ncolorsused %d", bmheader->colorsused);
 printf("\ncolorsimp %d", bmheader->colorsimp);
}




      /******************************************
      *
      *   read_color_table(...
      *
      *   This function reads the color table
      *   from a bmp image file.
      *
      ******************************************/

read_color_table(file_name, rgb, size)
   char   *file_name;
   struct ctstruct *rgb;
   int    size;
{
   int  i;
   char buffer[10];
   FILE *fp;

   fp = fopen(file_name, "rb");

   fseek(fp, 54, SEEK_SET);

   for(i=0; i<size; i++){
      fread(buffer, 1, 1, fp);
      rgb[i].blue = buffer[0];
      fread(buffer, 1, 1, fp);
      rgb[i].green = buffer[0];
      fread(buffer, 1, 1, fp);
      rgb[i].red = buffer[0];
      fread(buffer, 1, 1, fp);
         /* fourth byte nothing */
   }  /* ends loop over i */

   fclose(fp);

}  /* ends read_color_table */




      /******************************************
      *
      *   print_color_table(...
      *
      *   This function printfs the color table
      *   from a bmp file.
      *
      ******************************************/

print_color_table(struct ctstruct *rgb, int size)
{
   int i;

   for(i=0; i<size; i++){
      printf("\n %d %d %d",
      rgb[i].blue,rgb[i].green,rgb[i].red);
   }  /* ends loop over i */
}  /* ends print_color_table */






      /******************************************
      *
      *   flip_image_array(...
      *
      *   This function flips an image array
      *   about its horizontal mid-point.
      *
      ******************************************/

flip_image_array(the_image, rows, cols)
   long   cols, rows;
   short  **the_image;
{
   int  i, j;
   long rd2;
   short **temp;

   temp = allocate_image_array(rows, cols);
   rd2  = rows/2;
   for(i=0; i<rd2; i++){
      for(j=0; j<cols; j++){
         temp[rows-1-i][j] = the_image[i][j];
      }  /* ends loop over j */
   }  /* ends loop over i */

   for(i=rd2; i<rows; i++){
      for(j=0; j<cols; j++){
         temp[rows-1-i][j] = the_image[i][j];
      }  /* ends loop over j */
   }  /* ends loop over i */

   for(i=0; i<rows; i++)
      for(j=0; j<cols; j++)
         the_image[i][j] = temp[i][j];

   free_image_array(temp, rows);
}  /* ends flip_image_array */







      /******************************************
      *
      *   read_bmp_image(...
      *
      *   This function reads the image array
      *   from a bmp file.  
      *
      *   It only works for 8-bit images.
      *
      ******************************************/

read_bmp_image(file_name, array)
   char  *file_name;
   short **array;
{
   FILE   *fp;
   int    i, j;
   int    negative = 0,
          pad      = 0,
          place    = 0;
   long   colors   = 0,
          height   = 0,
          position = 0,
          width    = 0;
   struct bmpfileheader file_header;
   struct bitmapheader  bmheader;
   struct ctstruct rgb[GRAY_LEVELS+1];
   unsigned char uc;

   read_bmp_file_header(file_name, &file_header);
   read_bm_header(file_name, &bmheader);
   if(bmheader.bitsperpixel != 8){
      printf("\nCannot read image when bits per"
      "pixel is not 8");
      exit(1);
   }

   if(bmheader.colorsused == 0)
      colors = GRAY_LEVELS + 1;
   else
      colors = bmheader.colorsused;
   read_color_table(file_name, &rgb, colors);

   fp = fopen(file_name, "rb");
   fseek(fp, file_header.bitmapoffset, SEEK_SET);

   width = bmheader.width;
   if(bmheader.height < 0){
      height   = bmheader.height * (-1);
      negative = 1;
   }
   else
      height = bmheader.height;

   pad = calculate_pad(width);

   for(i=0; i<height; i++){
      for(j=0; j<width; j++){
         place = fgetc(fp);
         uc = (place & 0xff);
         place = uc;
         array[i][j] = rgb[place].blue;
      }  /* ends loop over j */
      if(pad != 0){
         position = fseek(fp, pad, SEEK_CUR);
      }  /* ends if pad 1= 0 */
   }  /* ends loop over i */

   if(negative == 0)
      flip_image_array(array, height, width);

}  /* ends read_bmp_image */






   /*********************************************
   *
   *   create_allocate_bmp_file(...
   *
   *   The calling routine must set the 
   *   height and width.  This routine will set
   *   everything else.
   *
   **********************************************/

create_allocate_bmp_file(file_name,
                         file_header,
                         bmheader)
   char  *file_name;
   struct bmpfileheader *file_header;
   struct bitmapheader  *bmheader;
{
   char buffer[100];
   int  i, pad = 0;
   FILE *fp;

   pad = calculate_pad(bmheader->width);

   bmheader->size         =  40;
   bmheader->planes       =   1;
   bmheader->bitsperpixel =   8;
   bmheader->compression  =   0;
   bmheader->sizeofbitmap = bmheader->height * 
                            (bmheader->width + pad);
   bmheader->horzres      = 300;
   bmheader->vertres      = 300;
   bmheader->colorsused   = 256;
   bmheader->colorsimp    = 256;

   file_header->filetype     = 0x4D42;
   file_header->reserved1    =  0;
   file_header->reserved2    =  0;
   file_header->bitmapoffset = 14 + 
                               bmheader->size +
                               bmheader->colorsused*4;
   file_header->filesize     = file_header->bitmapoffset +
                               bmheader->sizeofbitmap;

   if((fp = fopen(file_name, "wb")) == NULL){
      printf("\nERROR Could not create file %s",
             file_name);
      exit(2);
   }

      /*********************************************
      *
      *   Write the 14-byte bmp file header.
      *
      *********************************************/

   insert_ushort_into_buffer(buffer, 0, file_header->filetype);
   fwrite(buffer, 1, 2, fp);

   insert_ulong_into_buffer(buffer, 0, file_header->filesize);
   fwrite(buffer, 1, 4, fp);

   insert_short_into_buffer(buffer, 0, file_header->reserved1);
   fwrite(buffer, 1, 2, fp);

   insert_short_into_buffer(buffer, 0, file_header->reserved2);
   fwrite(buffer, 1, 2, fp);

   insert_ulong_into_buffer(buffer, 0, file_header->bitmapoffset);
   fwrite(buffer, 1, 4, fp);


      /*********************************************
      *
      *   Write the 40-byte bit map header.
      *
      *********************************************/

   insert_ulong_into_buffer(buffer, 0, bmheader->size);
   fwrite(buffer, 1, 4, fp);

   insert_long_into_buffer(buffer, 0, bmheader->width);
   fwrite(buffer, 1, 4, fp);

   insert_long_into_buffer(buffer, 0, bmheader->height);
   fwrite(buffer, 1, 4, fp);

   insert_ushort_into_buffer(buffer, 0, bmheader->planes);
   fwrite(buffer, 1, 2, fp);

   insert_ushort_into_buffer(buffer, 0, bmheader->bitsperpixel);
   fwrite(buffer, 1, 2, fp);

   insert_ulong_into_buffer(buffer, 0, bmheader->compression);
   fwrite(buffer, 1, 4, fp);

   insert_ulong_into_buffer(buffer, 0, bmheader->sizeofbitmap);
   fwrite(buffer, 1, 4, fp);

   insert_ulong_into_buffer(buffer, 0, bmheader->horzres);
   fwrite(buffer, 1, 4, fp);

   insert_ulong_into_buffer(buffer, 0, bmheader->vertres);
   fwrite(buffer, 1, 4, fp);

   insert_ulong_into_buffer(buffer, 0, bmheader->colorsused);
   fwrite(buffer, 1, 4, fp);

   insert_ulong_into_buffer(buffer, 0, bmheader->colorsimp);
   fwrite(buffer, 1, 4, fp);

      /*********************************************
      *
      *   Write a blank color table.
      *   It has 256 entries (number of colors)
      *   that are each 4 bytes.
      *
      *********************************************/

   buffer[0] = 0x00;

   for(i=0; i<(256*4); i++)
      fwrite(buffer, 1, 1, fp);

      /*********************************************
      *
      *   Write a zero image.  
      *
      *********************************************/

   buffer[0] = 0x00;

   for(i=0; i<bmheader->sizeofbitmap; i++)
      fwrite(buffer, 1, 1, fp);

   fclose(fp);

}  /* ends create_allocate_bmp_file */







      /******************************************
      *
      *   create_bmp_file_if_needed(...
      *
      *   This function allocates a bmp image
      *   file it it does not exist.  It uses
      *   the header information from the input
      *   image name.
      *
      ******************************************/

create_bmp_file_if_needed(in_name, out_name, out_image)
   char in_name[], out_name[];
   short **out_image;
{
   int    length, width;
   struct bmpfileheader file_header;
   struct bitmapheader  bmheader;

   if(does_not_exist(out_name)){
      printf("\n\n output file does not exist %s",
               out_name);
      read_bm_header(in_name, &bmheader);
      create_allocate_bmp_file(out_name, &file_header, &bmheader);
      printf("\nBFIN> Created %s", out_name);
   }  /* ends if does_not_exist */
}  /* ends bmp_file_if_needed */





      /******************************************
      *
      *   write_bmp_image(...
      *
      *   This function writes an image array
      *   to a bmp image file.
      *
      ******************************************/

write_bmp_image(file_name, array)
   char   *file_name;
   short  **array;
{
   char   *buffer, c;
   FILE   *image_file;
   int    pad = 0,
          position;
   int    bytes, i, j;
   long   height = 0, width = 0;
   struct bitmapheader  bmheader;
   struct bmpfileheader file_header;
   struct ctstruct rgb[GRAY_LEVELS+1];
   union  short_char_union scu;

   read_bmp_file_header(file_name, &file_header);
   read_bm_header(file_name, &bmheader);

   height = bmheader.height;
   width  = bmheader.width;
   if(height < 0) height = height*(-1);

   buffer = (char  *) malloc(width * sizeof(char ));
   for(i=0; i<width; i++)
      buffer[i] = '\0';

   image_file = fopen(file_name, "rb+");

      /****************************************
      *
      *   Write the color table first.
      *
      ****************************************/

   position   = fseek(image_file, 54, SEEK_SET);
   for(i=0; i<GRAY_LEVELS+1; i++){
      rgb[i].blue  = i;
      rgb[i].green = i;
      rgb[i].red   = i;
   }  /* ends loop over i */

   for(i=0; i<bmheader.colorsused; i++){
      buffer[0] = rgb[i].blue;
      fwrite(buffer , 1, 1, image_file);
      buffer[0] = rgb[i].green;
      fwrite(buffer , 1, 1, image_file);
      buffer[0] = rgb[i].red;
      fwrite(buffer , 1, 1, image_file);
      buffer[0] = 0x00;
      fwrite(buffer , 1, 1, image_file);
   }  /* ends loop over i */

   position   = fseek(image_file,
                      file_header.bitmapoffset, 
                      SEEK_SET);

   pad = calculate_pad(width);

   for(i=0; i<height; i++){
      for(j=0; j<width; j++){

         if(bmheader.bitsperpixel == 8){
            scu.s_num = 0;
            if(bmheader.height > 0)
               scu.s_num = array[height-1-i][j];
            else
               scu.s_num = array[i][j];
            buffer[j] = scu.s_alpha[0];
         }  /* ends if bits_per_pixel == 8 */
         else{
            printf("\nERROR bitsperpixel is not 8");
            exit(1);
         }
      }  /* ends loop over j */

      bytes = fwrite(buffer, 1, width, image_file);

      if(pad != 0){
         for(j=0; j<pad; j++)
            buffer[j] = 0x00;
         fwrite(buffer, 1, pad, image_file);
      }  /* ends if pad != 0 */

   }  /* ends loop over i */

   fclose(image_file);
   free(buffer);
}  /* ends write_bmp_image */





      /******************************************
      *
      *   calculate_pad(...
      *
      *   This function calculates the pad needed
      *   at the end of each row of pixels in a
      *   bmp image.
      *
      ******************************************/

int calculate_pad(width)
   long width;
{
   int pad = 0;
   pad = ( (width%4) == 0) ? 0 : (4-(width%4));
   return(pad);
}  /* ends calculate_pad */



   /**********************************************
   *
   *   get_image_size(...
   *
   *   This function reads the rows and cols
   *   from the header of either a tif or bmp
   *   image file.
   *
   *   IF IT CANNOT FIND THIS INFORMATION,
   *   it returns a ZERO.
   *
   ***********************************************/

int get_image_size(file_name, rows, cols)
   char *file_name;
   long *cols, *rows;
{
   int is_bmp  = 0,
       is_tiff = 0,
       result  = 0;
   struct bitmapheader bmph;
   struct tiff_header_struct tiffh;

   if(is_a_bmp(file_name)){
      is_bmp = 1;
      read_bm_header(file_name, &bmph);
      *rows = bmph.height;
      *cols = bmph.width;
   }  /* ends if is_a_bmp */

   if(is_a_tiff(file_name)){
      is_tiff = 1;
      read_tiff_header(file_name, &tiffh);
      *rows = tiffh.image_length;
      *cols = tiffh.image_width;
   }  /* ends if is_a_bmp */

   if(is_bmp == 1  ||  is_tiff == 1)
      result = 1;

   return(result);
}  /* ends get_image_size */






      /******************************************
      *
      *   get_bitsperpixel(...
      *
      *   This function reads the bits per pixel
      *   from either a tif or bmp image file.
      *
      ******************************************/

int get_bitsperpixel(file_name, bitsperpixel)
   char *file_name;
   long *bitsperpixel;
{
   int    is_bmp  = 0,
          is_tiff = 0,
          result  = 0;
   long   temp;
   struct bitmapheader bmph;
   struct tiff_header_struct tiffh;

   if(is_a_bmp(file_name)){
      is_bmp = 1;
      read_bm_header(file_name, &bmph);
      temp = (long)bmph.bitsperpixel;
      *bitsperpixel = temp;
   }  /* ends if is_a_bmp */

   if(is_a_tiff(file_name)){
      is_tiff = 1;
      read_tiff_header(file_name, &tiffh);
      *bitsperpixel = tiffh.bits_per_pixel;
   }  /* ends if is_a_bmp */

   if(is_bmp == 1  ||  is_tiff == 1)
      result = 1;

   return(result);
}  /* ends get_image_size */



   /**********************************************
   *
   *   is_a_bmp(...
   *
   *   This function looks at a file to see if it
   *   is a bmp file.  First look at the file
   *   extension.  Next look at the filetype to
   *   ensure it is 0x4d42.
   *
   ***********************************************/

int is_a_bmp(file_name)
   char *file_name;
{
   char   *cc;
   int    result = 0;
   struct bmpfileheader file_header;

   cc = strstr(file_name, ".bmp");
   if(cc == NULL)
      return(result);

   read_bmp_file_header(file_name, &file_header);
   if(file_header.filetype != 0x4d42)
      return(result);

   result = 1;
   return(result);
}  /* ends is_a_bmp */





   /**********************************************
   *
   *   is_a_tiff(...
   *
   *   This function looks at a file to see if it
   *   is a tiff file.  First look at the file
   *   extension.  Next look at the first four 
   *   bytes of the header.
   *
   ***********************************************/


int is_a_tiff(file_name)
   char *file_name;
{
   char   *cc;
   char   buffer[4];
   FILE   *fp;
   int    ok     = 0,
          result = 0;

   cc = strstr(file_name, ".tif");
   if(cc == NULL)
      return(result);

   fp = fopen(file_name, "rb");
   fread(buffer, 1, 4, fp);
   fclose(fp);

   if(buffer[0] == 0x49  &&
      buffer[1] == 0x49  &&
      buffer[2] == 0x2a  &&
      buffer[3] == 0x00)
      ok = 1;

   if(buffer[0] == 0x4d  &&
      buffer[1] == 0x4d  &&
      buffer[2] == 0x00  &&
      buffer[3] == 0x2a)
      ok = 1;

   if(ok == 0)
      return(result);

   result = 1;
   return(result);
}  /* ends is_a_tiff */




   /*******************************************
   *
   *   read_image_array(...
   *
   *   This routine reads the image data from
   *   either a tiff or bmp image.
   *
   ********************************************/

read_image_array(file_name, array)
   char  *file_name;
   short **array;
{
   int ok = 0;

   if(is_a_tiff(file_name)){
      read_tiff_image(file_name, array);
      ok = 1;
   }
   
   if(is_a_bmp(file_name)){
      read_bmp_image(file_name, array);
      ok = 1;
   }

   if(ok == 0){
      printf("\nERROR could not read file %s",
             file_name);
      exit(1);
   }

}  /* ends read_image_array */







   /*******************************************
   *
   *   write_image_array(...
   *
   *   This routine writes the image data to
   *   either a tiff or bmp image.
   *
   ********************************************/

write_image_array(file_name, array)
   char  *file_name;
   short **array;
{
   int ok = 0;

   if(is_a_tiff(file_name)){
      write_tiff_image(file_name, array);
      ok = 1;
   }
   
   if(is_a_bmp(file_name)){
      write_bmp_image(file_name, array);
      ok = 1;
   }

   if(ok == 0){
      printf("\nERROR could not write file %s",
             file_name);
      exit(1);
   }

}  /* ends write_image_array */



   /*********************************************
   *
   *   equate_tiff_headers(...
   *
   *   This function sets the elements of the
   *   destination header to the values of the
   *   source header.
   *
   *********************************************/

equate_tiff_headers(src, dest)
   struct tiff_header_struct *src, *dest;
{
   dest->lsb            = src->lsb;
   dest->bits_per_pixel = src->bits_per_pixel;
   dest->image_length   = src->image_length;
   dest->image_width    = src->image_width;
   dest->strip_offset   = src->strip_offset;
}  /* ends equate_tiff_headers */




   /*********************************************
   *
   *   equate_bmpfileheaders(...
   *
   *   This function sets the elements of the
   *   destination header to the values of the
   *   source header.
   *
   *********************************************/

equate_bmpfileheaders(src, dest)
   struct bmpfileheader *src, *dest;
{
   dest->filetype     = src->filetype;
   dest->filesize     = src->filesize;
   dest->reserved1    = src->reserved1;
   dest->reserved2    = src->reserved2;
   dest->bitmapoffset = src->bitmapoffset;
}  /* ends equate_bmpfileheaders */



   /*********************************************
   *
   *   equate_bitmapheaders(...
   *
   *   This function sets the elements of the
   *   destination header to the values of the
   *   source header.
   *
   *********************************************/

equate_bitmapheaders(src, dest)
   struct bitmapheader *src, *dest;
{
   dest->size         = src->size;
   dest->width        = src->width;
   dest->height       = src->width;
   dest->planes       = src->planes;
   dest->bitsperpixel = src->bitsperpixel;
   dest->compression  = src->compression;
   dest->sizeofbitmap = src->sizeofbitmap;
   dest->horzres      = src->horzres;
   dest->vertres      = src->vertres;
   dest->colorsused   = src->colorsused;
   dest->colorsimp    = src->colorsimp;
}  /* ends equate_bitmapheader */
