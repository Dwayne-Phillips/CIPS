
   /***********************************************
   *
   *   file showi.c
   *
   *   Functions: This file contains
   *      main
   *      show_screen
   *      is_in_image
   *
   *   Purpose:
   *      This file contains the program
   *      that shows image numbers on the screen.
   *
   *   External Calls:
   *      imageio.c - get_image_size
   *                  read_image_array
   *                  allocate_image_array
   *                  free_image_array
   *
   *   Modifications:
   *      1 October 1998 - created to work with
   *           all I O routines in imageio.c.
   *
   *************************************************/

#include "cips.h"
#define  SHEIGHT 20
#define  SWIDTH  15

int show_screen();
int is_in_image();
int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();


int main(argc, argv)
   int argc;
   char *argv[];
{
   char  in_name[MAX_NAME_LENGTH];
   char  response[MAX_NAME_LENGTH];
   int   ie, il, not_done, temp_ie, temp_il;
   long  height, width;
   short **the_image;

      /******************************************
      *
      *   Ensure the command line is correct.
      *
      ******************************************/

   if(argc != 4){
    printf("\nusage: showi input-image il ie\n");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   il = atoi(argv[2]);
   ie = atoi(argv[3]);

      /******************************************
      *
      *   Ensure the input image exists.
      *   Allocate an image array.
      *   Read the image and show it on the
      *   screen.
      *
      ******************************************/

   if(does_not_exist(in_name)){
      printf("\nERROR input file %s does not exist",
             in_name);
      exit(0);
   }  /* ends if does_not_exist */

   get_image_size(in_name, &height, &width);
   the_image = allocate_image_array(height, width);
   read_image_array(in_name, the_image);

   temp_il  = il;
   temp_ie  = ie;
   not_done = 1;

   while(not_done){
      if(is_in_image(temp_il, temp_ie, height, width)){
         il = temp_il;
         ie = temp_ie;
         show_screen(the_image, il, ie);
      }  /* ends if is_in_image */

      printf("\n\n x=quit j=down k=up h=left l=right"
             "\nEnter choice and press Enter:  ");
      gets(response);

      if(response[0] == 'x' || response[0] == 'X')
         not_done = 0;
      if(response[0] == 'j' || response[0] == 'J')
         temp_il = temp_il + ((3*SHEIGHT)/4);
      if(response[0] == 'k' || response[0] == 'K')
         temp_il = temp_il - ((3*SHEIGHT)/4);
      if(response[0] == 'h' || response[0] == 'H')
         temp_ie = temp_ie - ((3*SWIDTH)/4);
      if(response[0] == 'l' || response[0] == 'L')
         temp_ie = temp_ie + ((3*SWIDTH)/4);
   }  /* ends while not_done */

   free_image_array(the_image, height);

}  /* ends main */



int is_in_image(il, ie, height, width)
   int il, ie;
   long height, width;
{
   int result = 1;

   if(il < 0){
      printf("\nil=%d tool small", il);
      result = 0;
   }

   if(ie < 0){
      printf("\nie=%d tool small", ie);
      result = 0;
   }

   if((il+SHEIGHT) > height ){
      printf("\nll=%d tool big", il+SHEIGHT);
      result = 0;
   }

   if((ie+SWIDTH) > width ){
      printf("\nle=%d tool big", ie+SWIDTH);
      result = 0;
   }

   return(result);


}  /* ends is_in_image */


int show_screen(the_image, il, ie)
   int  il, ie;
   short **the_image;
{
   int i, j;

   printf("\n     ");
   for(i=ie-1; i<ie-1+SWIDTH; i++)
      printf("-%3d", i);

   for(i=il-1; i<il-1+SHEIGHT; i++){
      printf("\n%4d>", i);
      for(j=ie-1; j<ie-1+SWIDTH; j++){
         printf("-%3d", the_image[i][j]);
      }
   }
return(1);
}  /* ends show_screen */
