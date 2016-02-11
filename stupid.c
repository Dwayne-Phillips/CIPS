

/* silly change */
/* another silly change */

read_tiff_header(file_name, image_header)
   char file_name[];
   struct tiff_header_struct *image_header;
{
}  /* ends read_tiff_header */



short **allocate_image_array(length, width)
   long  length, width;
{
}  /* ends allocate_image_array */



int free_image_array(the_array, length)
   short **the_array;
   long  length;
{
}  /* ends free_image_array */



read_tiff_image(image_file_name, the_image)
      char   image_file_name[];
      short   **the_image;
{
}  /*  ends read_tiff_image */


read_line(image_file, the_image, line_number, 
          image_header, ie, le)
   FILE   *image_file;
   int    ie, le, line_number;
   short  **the_image;
   struct tiff_header_struct *image_header;
{
}  /* ends read_line  */


seek_to_first_line(image_file, image_header, il)
   FILE   *image_file;
   int    il;
   struct tiff_header_struct *image_header;
{
}  /* ends seek_to_first_line */


seek_to_end_of_line(image_file, le, image_header)
   FILE   *image_file;
   int    le;
   struct tiff_header_struct *image_header;
{
}  /* ends seek_to_end_of_line         */


create_file_if_needed(in_name, out_name, out_image)
   char in_name[], out_name[];
   short **out_image;
{
}  /* ends create_file_if_needed */



create_allocate_tiff_file(file_name, 
                          image_header)
   char   file_name[];
   struct tiff_header_struct *image_header;
{
}  /* ends create_allocate_tiff_file */



write_tiff_image(image_file_name, array)

        char    image_file_name[];
        short   **array;
{
}  /*  ends write_tiff_image */


write_line(image_file, array, line_number, 
           image_header, ie, le)
   FILE   *image_file;
   int    ie, le, line_number;
   short  **array;
   struct tiff_header_struct *image_header;
{
}  /* ends write_line  */



does_not_exist(file_name)
    char file_name[];
{
}  /* ends does_not_exist */



read_bmp_file_header(file_name,
                     file_header)
   char *file_name;
   struct bmpfileheader *file_header;
{
}  /* ends read_bmp_file_header */



print_bmp_file_header(struct bmpfileheader *file_header)
{
}



read_bm_header(file_name,
               bmheader)
   char *file_name;
   struct bitmapheader *bmheader;
{
}  /* ends read_bm_header */



print_bm_header(bmheader)
   struct bitmapheader *bmheader;
{
}



read_color_table(file_name, rgb, size)
   char   *file_name;
   struct ctstruct *rgb;
   int    size;
{
}  /* ends read_color_table */



print_color_table(struct ctstruct *rgb, int size)
{
}  /* ends print_color_table */



flip_image_array(the_image, rows, cols)
   long   cols, rows;
   short  **the_image;
{
}  /* ends flip_image_array */



read_bmp_image(file_name, array)
   char  *file_name;
   short **array;
{
}  /* ends read_bmp_image */


create_allocate_bmp_file(file_name,
                         file_header,
                         bmheader)
   char  *file_name;
   struct bmpfileheader *file_header;
   struct bitmapheader  *bmheader;
{
}  /* ends create_allocate_bmp_file */




create_bmp_file_if_needed(in_name, out_name, out_image)
   char in_name[], out_name[];
   short **out_image;
{
}  /* ends bmp_file_if_needed */



write_bmp_image(file_name, array)
   char   *file_name;
   short  **array;
{
}  /* ends write_bmp_image */



int calculate_pad(width)
   long width;
{
}  /* ends calculate_pad */



int get_image_size(file_name, rows, cols)
   char *file_name;
   long *cols, *rows;
{
}  /* ends get_image_size */




int get_bitsperpixel(file_name, bitsperpixel)
   char *file_name;
   long *bitsperpixel;
{
}  /* ends get_image_size */



int is_a_bmp(file_name)
   char *file_name;
{
}  /* ends is_a_bmp */




int is_a_tiff(file_name)
   char *file_name;
{
}  /* ends is_a_tiff */



read_image_array(file_name, array)
   char  *file_name;
   short **array;
{
}  /* ends read_image_array */




write_image_array(file_name, array)
   char  *file_name;
   short **array;
{
}  /* ends write_image_array */




equate_tiff_headers(src, dest)
   struct tiff_header_struct *src, *dest;
{
}  /* ends equate_tiff_headers */




equate_bmpfileheaders(src, dest)
   struct bmpfileheader *src, *dest;
{
}  /* ends equate_bmpfileheaders */




equate_bitmapheaders(src, dest)
   struct bitmapheader *src, *dest;
{
}  /* ends equate_bitmapheader */




int are_not_same_size(file1, file2)
   char *file1, *file2;
{
}  /* ends are_not_same_size */

