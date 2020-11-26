
       /*********************************************
       *
       *   file mainl.c
       *
       *   Functions: This file contains
       *      main
       *      show_mainl_usage
       *
       *   Purpose:
       *      This file contains the main calling
       *      routine that calls the lambert
       *      functions.
       *
       *   External Calls:
       *      imageio.c - create_image_file
       *                  read_image_array
       *                  write_image_array
       *                  get_image_size
       *                  allocate_image_array
       *                  free_image_array
       *
       *   Modifications:
       *      26 November 1999 - created
       *
       ***********************************************/

#include "cips.h"

int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();
int show_mainl_usage();
int lambert();



int main(argc, argv)
   int argc;
   char *argv[];
{

   char     name1[80], name2[80];
   float    k_diffuse, k_specular, eta, L[3];
   long     length, width;
   short    **the_image;
   short    **out_image;


       /****************************************
       *
       *   Interpret the command line parameters.
       *
       *****************************************/

   if(argc != 9){
      show_mainl_usage();
      exit(0);
   }

   strcpy(name1, argv[1]);
   strcpy(name2, argv[2]);
   k_diffuse  = atof(argv[3]);
   k_specular = atof(argv[4]);
   eta        = atof(argv[5]);
   L[0]       = atof(argv[6]);
   L[1]       = atof(argv[7]);
   L[2]       = atof(argv[8]);

   if(does_not_exist(name1)){
    printf("\nERROR input file %s does not exist",
             name1);
    exit(0);
   }


       /****************************************
       *
       *   Allocate the arrays, create the output
       *   file, read data.
       *
       *****************************************/

   get_image_size(name1, &length, &width);

   the_image = allocate_image_array(length, width);
   out_image = allocate_image_array(length, width);

   create_image_file(name1, name2);
   read_image_array(name1, the_image);

       /*********************************************
       *
       *   Call the desired function.
       *
       **********************************************/


   lambert(the_image, out_image,
           k_diffuse, k_specular, eta, L,
           length, width);

       /*********************************************
       *
       *   Write the result, free the arrays, end
       *
       **********************************************/

   write_image_array(name2, out_image);

   free_image_array(out_image, length);
   free_image_array(the_image, length);

}  /* ends main  */






int show_mainl_usage()
{
   char response[80];

printf(
"\n\nNot enough parameters:"
"\n"
"\n usage: mainl in-file k_diffuse k_specular eta"
" Light-x y z (n.n)"
" "
"\n");
return(1);
}  /* ends show_mainl_usage */
