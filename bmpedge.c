



#include "cips.h"



struct bmpfileheader{
   unsigned short  filetype;
   unsigned long   filesize;
   short  reserved1;
   short  reserved2;
   unsigned long   bitmapoffset;
};

struct bitmapheader{
   unsigned long   size;
   long   width;
   long   height;
   unsigned short  planes;
   unsigned short  bitsperpixel;
   unsigned long   compression;
   unsigned long   sizeofbitmap;
   unsigned long   horzres;
   unsigned long   vertres;
   unsigned long   colorsused;
   unsigned long   colorsimp;
};

struct ctstruct{
   unsigned char blue;
   unsigned char green;
   unsigned char red;
};





int main(argc, argv)
   int argc;
   char *argv[];
{
   char  image_name[MAX_NAME_LENGTH];
   char  image_name2[MAX_NAME_LENGTH];
   char  response[MAX_NAME_LENGTH];
   int   i, j;
   short **the_image, **out_image;
   struct   tiff_header_struct image_header;


   struct bmpfileheader file_header;
   struct bitmapheader  bmheader;
   struct ctstruct      rgb[256];

   long   height, width;


      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc < 3){
    printf("\n   usage: bmedge input-image output-image");
    exit(0);
   }
   else
      printf("\n%s %s %s", argv[0], argv[1], argv[2]);

   strcpy(image_name, argv[1]);
   strcpy(image_name2, argv[2]);

   read_bmp_file_header(image_name, &file_header);
   read_bm_header(image_name, &bmheader);

   width = bmheader.width;
   if(bmheader.height < 0)
      height   = bmheader.height * (-1);
   else
      height = bmheader.height;

   the_image = allocate_image_array(height, width);
   out_image = allocate_image_array(height, width);
   read_bmp_image(image_name, the_image);


   create_bmp_file_if_needed(image_name, image_name2, the_image);

   read_bmp_image(image_name, the_image);
printf("\nMAIN> read %s", image_name);

   quick_edge(the_image,
              out_image,
              1,         /* threshold */
              90,        /* high      */
              bmheader.height,
              bmheader.width,
              bmheader.bitsperpixel);


printf("\nMAIN> Back from the edge detector");


   write_bmp_image(image_name2, out_image);
printf("\nMAIN> wrote %s", image_name2);

   free_image_array(the_image,
      image_header.image_length);
   free_image_array(out_image,
      image_header.image_length);

}  /* ends main */










int read_bmp_file_header(file_name,
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


int print_bmp_file_header(struct bmpfileheader *file_header)
{
 printf("\nfile type %x", file_header->filetype);
 printf("\nfile size %d", file_header->filesize);
 printf("\nbit map offset %d", file_header->bitmapoffset);
}



int read_bm_header(file_name,
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


int print_bm_header(struct bitmapheader *bmheader)
{
 printf("\nwidth %d", bmheader->width);
 printf("\nheight %d", bmheader->height);
 printf("\nplanes %d", bmheader->planes);
 printf("\nbitsperpixel %d", bmheader->bitsperpixel);
 printf("\ncolorsused %d", bmheader->colorsused);
 printf("\ncolorsimp %d", bmheader->colorsimp);
}



int read_color_table(file_name, rgb, size)
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



int print_color_table(struct ctstruct *rgb, int size)
{
   int i;

   for(i=0; i<size; i++){
      printf("\n %d %d %d",
      rgb[i].blue,rgb[i].green,rgb[i].red);
   }  /* ends loop over i */
}  /* ends print_color_table */





int flip_image_array(the_image, rows, cols)
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






int read_bmp_image(file_name, array)
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

   if(negative = 0)
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

int create_allocate_bmp_file(file_name,
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






int create_bmp_file_if_needed(in_name, out_name, out_image)
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
}  /* ends bcreate_file_if_needed */




int write_bmp_image(file_name, array)
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




int calculate_pad(width)
   long width;
{
   int pad = 0;
   pad = ( (width%4) == 0) ? 0 : (4-(width%4));
   return(pad);
}  /* ends calculate_pad */
