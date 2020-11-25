
       /*********************************************
       *
       *   file boolean.c
       *
       *   Functions: This file contains
       *      main
       *
       *   Purpose:
       *      This file contains the main calling
       *      routine that calls the Boolean
       *      operations.
       *
       *   External Calls:
       *      imageio.c - create_image_file
       *                  read_image_array
       *                  write_image_array
       *                  get_image_size
       *                  allocate_image_array
       *                  free_image_array
       *      boole.c - and_image
       *                or_image
       *                xor_image
       *                nand_image
       *                nor_image
       *                not_image
       *
       *   Modifications:
       *      3 March 1993 - created
       *      22 August 1998 - modified to work on
       *           entire images at once.
       *      19 September 1998 - modified to work with
       *           all I O routines in imageio.c.
       *
       *********************************************/

#include "cips.h"

int does_not_exist();
int get_image_size();
int create_image_file();
int read_image_array();
int free_image_array();
int not_image();
int write_image_array();
int are_not_same_size();
int and_image();
int or_image();
int xor_image();
int nand_image();
int nor_image();

short **the_image;
short **out_image;

int main(argc, argv)
   int argc;
   char *argv[];
{

   char     name1[80], name2[80], name3[80], type[80];
   long     length, width;
   short    value;

       /****************************************
       *
       *   Interpret the command line parameters.
       *
       *****************************************/

   if(argc < 5){
    printf(
    "\n\nNot enough parameters:"
     "\n"
     "\n usage: boolean in-file1 in-file2 out-file "
     "type [value]"
     "\n           or "
     "\n        boolean in-file1 out-file not value"
     "\n"
     "\n recall type: and or xor nand nor not"
     "\n              You must specify a value for "
     "nand & nor"
     "\n");
    exit(0);
   }

   if(strcmp("not", argv[3]) == 0){
      strcpy(name1, argv[1]);
      strcpy(name2, argv[2]);
      strcpy(type,  argv[3]);
      value =  atoi(argv[4]);
   }  /* ends if not */
   else{
      strcpy(name1, argv[1]);
      strcpy(name2, argv[2]);
      strcpy(name3, argv[3]);
      strcpy(type,  argv[4]);
      value =  atoi(argv[5]);
   }  /* ends else all other types */

   if(does_not_exist(name1)){
    printf("\nERROR input file %s does not exist",
             name1);
    exit(0);
   }

       /****************************************
       *
       *  Process the NOT case (1 input file,
       *  1 output file).
       *
       *  Else process the other cases.
       *
       *****************************************/

       /*  NOT CASE */

   if(strcmp("not", type) == 0){
      get_image_size(name1, &length, &width);
      the_image = allocate_image_array(length, width);
      out_image = allocate_image_array(length, width);
      create_image_file(name1, name2);
      read_image_array(name1, the_image);
      not_image(the_image, out_image, value,
                length, width);
      write_image_array(name2, out_image);
   }  /* ends if not case */



       /* NOW ALL OTHER CASES */

   else{

      if(does_not_exist(name2)){
       printf("\nERROR input file %s does not exist",
                name2);
       exit(0);
      }

      if(are_not_same_size(name1, name2)){
         printf(
          "\n Images %s and %s are not the same size",
          name1, name2);
         exit(1);
      }  /* ends if sizes not the same */

      get_image_size(name1, &length, &width);
      the_image = allocate_image_array(length, width);
      out_image = allocate_image_array(length, width);
      create_image_file(name1, name3);
      read_image_array(name1, the_image);
      read_image_array(name2, out_image);
      

            /* AND */
      if(strcmp("and", type) == 0){
         and_image(the_image, out_image,
                   length,
                   width);
      }  /* ends AND operation */

            /* OR */
      if(strcmp("or", type) == 0){
         or_image(the_image, out_image,
                  length,
                  width);
      }  /* ends OR operation */

            /* XOR */
      if(strcmp("xor", type) == 0){
         xor_image(the_image, out_image,
                   length,
                   width);
      }  /* ends XOR operation */

            /* NAND */
      if(strcmp("nand", type) == 0){
         nand_image(the_image, out_image,
                    value,
                    length,
                    width);
      }  /* ends NAND operation */

            /* NOR */
      if(strcmp("nor", type) == 0){
         nor_image(the_image, out_image,
                   value,
                   length,
                   width);
      }  /* ends NOR operation */

      write_image_array(name3, out_image);

   }  /* ends else all other cases (not not) */


   free_image_array(out_image, length);
   free_image_array(the_image, length);


}  /* ends main  */
