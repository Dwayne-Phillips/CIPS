

   /**************************************************
   *
   *   file ht.c
   *
   *   Functions: This file contains
   *       display_using_halftoning
   *       half_tone
   *       show_half_tone
   *       get_threshold_value
   *
   *   Purpose: This program displays an image using
   *      a halftoning process.  The algorithm was
   *      taken from "Personal computer based image
   *      processing with halftoning," John A Saghri,
   *      Hsieh S. Hou, Andrew Tescher, Optical
   *      Engineering, March 1986, Vol.25, No. 3,
   *      pp 499-503. The display_using_halftoning
   *      determines display size and reads the image.
   *      The half_tone function implements the
   *      algorithm shown on page 502 of the article.
   *
   *      The function print_halftone_array prints
   *      a half toned image array to a regular line
   *      printer.
   *
   *
   *   External Calls:
   *      rtiff.c - read_tiff_image
   *      numcvrt.c - get_integer
   *
   *   Modifications:
   *      30 September 86 - created
   *      18 August 1990 - modified for use in the
   *          C Image Processing System.
   *
   *
   **************************************************/


#include "cips.h"

#define  FORMFEED  '\014'



       /*********************************************
       *
       *   half_tone(...
       *
       *   ep[m][n] = sum of erros propogated
       *               to position (m,n).
       *   eg[m][n] = total error generated at
       *               location (m,n).
       *
       **********************************************/

int half_tone(in_image, out_image,
          threshold,
          one, zero,
          rows, cols)
   long  rows, cols;
   short threshold, one, zero;
   short **in_image, **out_image;
{
   float **eg, **ep;
   float c[2][3],
         sum_p,
         t;
   int   i, j, m, n, xx, yy;

   c[0][0] = 0.0;
   c[0][1] = 0.2;
   c[0][2] = 0.0;
   c[1][0] = 0.6;
   c[1][1] = 0.1;
   c[1][2] = 0.1;

      /***********************************************
      *
      *   Calculate the total propogated error
      *   at location(m,n) due to prior
      *   assignment.
      *
      *   Go through the input image.  If the output
      *   should be one then display that pixel as such.
      *   If the output should be zero then display it
      *   that way.
      *
      *   Also set the pixels in the input image array
      *   to 1's and 0's in case the print option
      *   was chosen.
      *
      ************************************************/

   eg = malloc(rows * sizeof(float  *));
   for(i=0; i<rows; i++){
      eg[i] = malloc(cols * sizeof(float ));
      if(eg[i] == NULL){
         printf("\n\tmalloc of eg[%d] failed", i);
      }  /* ends if */
   }  /* ends loop over i */

   ep = malloc(rows * sizeof(float  *));
   for(i=0; i<rows; i++){
      ep[i] = malloc(cols * sizeof(float ));
      if(ep[i] == NULL){
         printf("\n\tmalloc of ep[%d] failed", i);
      }  /* ends if */
   }  /* ends loop over i */

   for(i=0; i<rows; i++){
      for(j=0; j<cols; j++){
         eg[i][j] = 0.0;
         ep[i][j] = 0.0;
      }
   }


       /**********************************************
       *
       *   29 February 1988 - Fix to remove a solid
       *   line at the bottom of each region. Loop
       *   over ROWS-1 and then draw an extra line.
       *
       **********************************************/

   for(m=0; m<rows; m++){
      for(n=0; n<cols; n++){

         sum_p = 0.0;
         for(i=0; i<2; i++){
            for(j=0; j<3; j++){

               xx = m-i+1;
               yy = n-j+1;
               if(xx < 0)     xx = 0;
               if(xx >= rows) xx = rows-1;
               if(yy < 0)     yy = 0;
               if(yy >= cols) yy = cols-1;

               sum_p = sum_p + c[i][j] * eg[xx][yy];
            }  /* ends loop over j */
         }     /* ends loop over i */

         ep[m][n] = sum_p;
         t = in_image[m][n] + ep[m][n];

               /**********************************
               *
               *    Here set the point [m][n]=one
               *
               ***********************************/

         if(t > threshold){
            eg[m][n]        = t - threshold*2;
            out_image[m][n] = one;
         }  /* ends if t > threshold  */


               /**********************************
               *
               *    Here set the point [m][n]=zero
               *
               ***********************************/

         else{  /* t <= threshold */
            eg[m][n]        = t;
            out_image[m][n] = zero;
         }  /* ends else t <= threshold  */

      }  /* ends loop over n columns */
   }         /* ends loop over m rows */


   for(i=0; i<rows; i++){
      free(eg[i]);
      free(ep[i]);
   }

return(1);
}  /*  ends half_tone  */









#ifdef NEVER
float eg[ROWS][COLS], ep[ROWS][COLS];

display_using_halftoning(in_image, file_name,
         il, ie, ll, le, threshold, invert,
         image_colors, image_header, monitor_type,
         print, show_hist, color_transform)

   char  color_transform[], file_name[],
         monitor_type[];
   int   image_colors, invert,
         il, ie, ll, le, threshold,
         print, show_hist;
   short in_image[ROWS][COLS];
   struct tiff_header_struct *image_header;

{
   char response[80];

   int  a,
        b,
        channel,
        color,
        count,
        data_color,
        display_mode,
        horizontal,
        i,
        j,
        k,
        l,
        line_color,
        max_horizontal,
        max_vertical,
        not_finished,
        one,
        vertical,
        x,
        x_offset,
        y,
        y_offset,
        zero;

   float area, new_grays;

   unsigned long histogram[256], new_hist[256];


       if(  (show_hist == 1)  &&
            (color_transform[0] != 'H'))
          zero_histogram(histogram);

         /*******************************************
         *
         *   Use the monitor type to set the vertical
         *   horizontal and display_mode parameters.
         *   Also set the values for one and zero.
         *   one and zero will vary depending on the
         *   monitor type.
         *
         ********************************************/


      if(  (monitor_type[0] == 'M')  ||
           (monitor_type[0] == 'm')){
         vertical     = 3;
         horizontal   = 2;
         display_mode = HRESBW;
         one          = 1;
         zero         = 0;
      }

      if(  (monitor_type[0] == 'C')  ||
           (monitor_type[0] == 'c')){
         vertical     = 3;
         horizontal   = 2;
         display_mode = MRES4COLOR;
         one          = 3;
         zero         = 1;
      }

      if(  (monitor_type[0] == 'V')  ||
           (monitor_type[0] == 'v')){
         vertical     = 6;
         horizontal   = 4;
         display_mode = VRES16COLOR;
         one          = 5;
         zero         = 1;
      }

      if(  (monitor_type[0] == 'E')  ||
           (monitor_type[0] == 'e')){
         vertical     = 6;
         horizontal   = 3;
         display_mode = ERESCOLOR;
         one          = 5;
         zero         = 1;
      }

      max_horizontal = (image_header->image_length+50)
                         /COLS;
      max_vertical   = (image_header->image_width+50)
                         /ROWS;

      if(horizontal > max_horizontal)
            horizontal = max_horizontal;
      if(vertical > max_vertical)
            vertical = max_vertical;

      if(print == 1){
         vertical   = 1;
         horizontal = 1;
      }



        /****************************************
        *
        *   If color transform wants histogram
        *   equalization, then read in the
        *   image arrays and calculate the
        *   histogram.   Zero both the histogram
        *   and the new_hist.  You will need the
        *   new_hist if you want to display the
        *   equalized hist.
        *
        *****************************************/

      if(color_transform[0] == 'H'){
         count = 1;
         zero_histogram(histogram);
         zero_histogram(new_hist);
         for(a=0; a<vertical; a++){
            for(b=0; b<horizontal; b++){

               x = a*COLS;
               y = b*ROWS;

               printf("\nHT> Calculating histogram");
               printf(" %d of %d",
                      count,vertical*horizontal);
               count++;

               read_tiff_image(file_name, in_image,
                            il+y, ie+x, ll+y, le+x);
               calculate_histogram(in_image,
                                   histogram);

            }  /* ends loop over b */
         }  /* ends loop over a */
      }  /* ends if display_mode == H */




           /* set graphics mode */
      if(print == 0)
         my_setvideomode(display_mode); /* MSC 6.0 */
      else{
         printf("\n\nHT> Calculating for printing ");
         printf("\nHT> Counting from 0 to 99\n");
      }

        /********************************************
        *
        *   Loop over horizontal and vertical. Read
        *   the image array and display it after
        *   calculating the half tone values.
        *
        *
        *   If you want to show the histogram AND
        *   do not want to do hist equalization
        *   then calculate the hist from the
        *   original image array.
        *
        *   If you want to do hist equalization
        *   then calculate the new_hist AFTER
        *   the image has been equalized.
        *
        *   We will equalize the histogram down
        *   to half the original shades of gray
        *   and will cut the threshold in half.
        *
        *****************************************/



      for(i=0; i<horizontal; i++){
         for(j=0; j<vertical; j++){
            read_tiff_image(file_name, in_image,
                           il+i*ROWS, ie+j*COLS,
                           ll+i*ROWS, le+j*COLS);

            if(   (show_hist == 1)  &&
                  (color_transform[0] != 'H'))
               calculate_histogram(in_image, histogram);

            if(color_transform[0] == 'H'){

               area = ((long)(vertical))*
                      ((long)(horizontal));
               area = area*10000.0;
               new_grays = image_colors/2;

               perform_histogram_equalization(in_image,
                        histogram, new_grays, area);

               calculate_histogram(in_image, new_hist);

            }  /* ends if color_transform == S */

            if(color_transform[0] == 'H')
               half_tone(in_image, threshold/2,
                      eg, ep, i, j,
                      one, zero, invert, print);

            else
               half_tone(in_image, threshold,
                      eg, ep, i, j,
                      one, zero, invert, print);

         }  /* ends loop over j */
      }  /*  ends loop over i */



         /***************************
         *
         *   if show_hist == 1 then
         *   display the histogram
         *   in the lower right hand
         *   corner of screen
         *
         ****************************/

      if(  (show_hist == 1)   &&
           (print == 0)){

         if(monitor_type[0] == 'V'){
            y_offset   = 470;
            x_offset   = 380;
            line_color = 3;
            data_color = 8;
         }

         if(monitor_type[0] == 'E'){
            y_offset   = 310;
            x_offset   = 380;
            line_color = 3;
            data_color = 8;
         }

         if(monitor_type[0] == 'M'){
            y_offset   = 190;
            x_offset   = 60;
            line_color = 1;
            data_color = 1;
         }

         if(monitor_type[0] == 'C'){
            y_offset   = 190;
            x_offset   = 60;
            line_color = 1;
            data_color = 3;
         }

         if(color_transform[0] == 'S')
            display_histogram(histogram, x_offset,
                   y_offset, line_color, data_color);

         if(color_transform[0] == 'H')
            display_histogram(new_hist, x_offset,
                   y_offset, line_color, data_color);

      }  /* ends if show_hist == 1 and print == 0 */



      if(print == 1)
         printf("\n\nHT> Hit ENTER to continue");
      gets(response);
      my_clear_text_screen();

}  /* ends main  */


#endif



#ifdef NEVER


   /*******************************
   *
   *   get_threshold_value(...
   *
   ********************************/

get_threshold_value(threshold, print)
   int *print, *threshold;
{
   int i;
   printf("\nHT> The threshold = %d", *threshold);
   printf("\nHT> Enter new theshold value ");
   printf("(0 for no change) \n___\b\b\b");
   get_integer(&i);
   if((i != 0) && (i!= *threshold))
      *threshold = i;

   printf(
      "\nHT> print = %d (1 for print  0 for display)",
      *print);
   printf("\nHT> Enter print value  \n_\b");
   get_integer(&i);
   *print = i;
}




   /********************************************
   *
   *   print_halftone_array(...
   *
   *   This function takes the halftoned images
   *   and prints it to a line printer.  If the
   *   image array has a 1 then print a ' '.
   *   If the image array has a 0 then print
   *   a '*'.
   *
   *********************************************/


print_halftone_array(image)
   short image[ROWS][COLS];
{
   char printer_name[80], response[80], string[101];
   FILE *printer;
   int        i, j, l, line_counter;

   line_counter = 0;

   strcpy(printer_name, "prn");

   if( (printer = fopen(printer_name, "w")) == NULL)
      printf("\nHT> Could not open printer");
   else{
      printf("\nOpened printer and now printing");

    /*************************************************
    *
    *   Loop over the rows in the image.  For each row
    *   first clear out the string print buffer.
    *   Then go through the columns and set the string
    *   to either '*' or ' '.  Finally, write the
    *   string out to the printer.
    *
    *************************************************/

   for(i=0; i<ROWS; i++){

      for(l=0; l<COLS+1; l++) string[l] = '\0';

      for(j=0; j<COLS; j++){
         if(image[i][j] == 1)
            string[j] = '*';
         else
            string[j] = ' ';
      }  /* ends loop over j columns */

      printf("%3d", i);
      fputs(string, printer);
      fputc('\n', printer);
      line_counter = line_counter + 1;
      if(line_counter >= 53){
         line_counter = 0;
         putc(FORMFEED, printer);
      }  /* ends if line_counter >=53  */

   }  /* ends loop over i rows */


   }  /* ends opened printer */

   putc(FORMFEED, printer);
   fclose(printer);

}  /* ends print_halftone_array */



#endif
