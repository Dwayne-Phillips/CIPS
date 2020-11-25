
    /********************************************
    *
    *    file d:\cips\geometry.c
    *
    *    Functions: This file contains
    *       main
    *
    *    Purpose:
    *       This file contains the main calling
    *       routine for geometric subroutines.
    *
    *    External Calls:
    *      imageio.c - create_image_file
    *                  read_image_array
    *                  write_image_array
    *                  get_image_size
    *                  allocate_image_array
    *                  free_image_array
    *       geosubs.c - geometry
    *                   arotate
    *
    *    Modifications:
    *       26 October 1993 - created
    *       27 August 1998 - modified to work on
    *            entire images at once.
    *       19 September 1998 - modified to work with
    *           all I O routines in imageio.c.
    *
    ********************************************/

#include "cips.h"

int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();
int geometry();
int arotate();

short **the_image;
short **out_image;

int main(argc, argv)
   int argc;
   char *argv[];
{

   char   name1[80], name2[80], type[80];
   float  theta, x_stretch, y_stretch,
          x_cross, y_cross;
   int    bilinear;
   int    x_control, y_control;
   long   length, width;
   short  m, n, x_displace, y_displace;

      /*************************************
      *
      *   This program will use a different
      *   command line for each type of
      *   call.
      *
      *   Print a usage statement that
      *   gives an example of each type
      *   of call.
      *
      *************************************/

   if(argc < 7){
    printf("\n\nNot enough parameters:");
    printf("\n");
    printf("\n   Two Operations: ");
    printf("\n      geometry  rotate");
    printf("\n\n   Examples:");
    printf("\n");
    printf("\n   geometry in out geometry angle");
    printf(" x-displace y-displace");
    printf("\n            x-stretch y-stretch");
    printf(" x-cross y-cross bilinear (1 or 0)");
    printf("\n");
    printf("\n   geometry in out rotate angle m n");
    printf(" bilinear (1 or 0)");
    printf("\n");
    exit(0);
   }


      /*************************************
      *
      *   Interpret the command line
      *   depending on the type of call.
      *
      *************************************/

   if(strncmp(argv[3], "geometry", 3) == 0){
      strcpy(name1,  argv[1]);
      strcpy(name2, argv[2]);
      strcpy(type,  argv[3]);
      theta      = atof(argv[4]);
      x_displace = atoi(argv[5]);
      y_displace = atoi(argv[6]);
      x_stretch  = atof(argv[7]);
      y_stretch  = atof(argv[8]);
      x_cross    = atof(argv[9]);
      y_cross    = atof(argv[10]);
      bilinear   = atoi(argv[11]);
   }


   if(strncmp(argv[3], "rotate", 3) == 0){
      strcpy(name1,  argv[1]);
      strcpy(name2, argv[2]);
      strcpy(type,  argv[3]);
      theta    = atof(argv[4]);
      m        = atoi(argv[5]);
      n        = atoi(argv[6]);
      bilinear = atoi(argv[7]);
   }

   if(does_not_exist(name1)){
    printf("\nERROR input file %s does not exist",
             name1);
    exit(0);
   }

   get_image_size(name1, &length, &width);
   the_image = allocate_image_array(length, width);
   out_image = allocate_image_array(length, width);
   create_image_file(name1, name2);
   read_image_array(name1, the_image);

      /*************************************
      *
      *   Call the routines
      *
      *************************************/

   if(strncmp(type, "geometry", 3) == 0){
     geometry(the_image, out_image,
             theta, x_stretch, y_stretch,
             x_displace, y_displace,
             x_cross, y_cross,
             bilinear,
             length,
             width);
   }  /* ends if */



   if(strncmp(type, "rotate", 3) == 0){
     arotate(the_image, out_image,
            theta, m, n, bilinear,
            length,
            width);
   }  /* ends if */


   write_image_array(name2, out_image);
   free_image_array(out_image, length);
   free_image_array(the_image, length);

}  /* ends main  */
