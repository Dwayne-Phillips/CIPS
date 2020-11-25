

       /*********************************************
       *
       *       file flip.c
       *
       *       Functions: This file contains
       *          main
       *          flip_image
       *
       *       Purpose:
       *          The program rotates or flips an
       *          image in 90 degree increments.
       *
       *
       *   The rotation_type parameter specifies
       *   the operation.  When rotation_type is
       *   1, 2, or 3 you rotate.  When it is
       *   4 or 5 you flip.
       *
       *   I define rotation as this:  Pin down the
       *   lower left hand corner of the image array
       *   and rotate the image 90 degrees clockwise.
       *   1 rotation is 90 degrees, 2 rotations are
       *   180 degrees, and 3 rotations are 270 degrees.
       *   4 rotations bring you back to where you
       *   started.
       *
       *   The cases are:
       *
       *   If the input image array is:
       *        1 2 3
       *        4 5 6
       *        7 8 9
       *
       *   Rotate # 1 - the result is:
       *        7 4 1
       *        8 5 2
       *        9 6 3
       *
       *   Rotate # 2 - the result is:
       *        9 8 7
       *        6 5 4
       *        3 2 1
       *
       *   Rotate # 3 - the result is:
       *        3 6 9
       *        2 5 8
       *        1 4 7
       *
       *   Flip # 4 - horizontal the result is:
       *        3 2 1
       *        6 5 4
       *        9 8 7
       *
       *   Flip # 5 - vertical the result is:
       *        7 8 9
       *        4 5 6
       *        1 2 3
       *
       *
       *       External Calls:
       *          imageio.c - create_image_file
       *                  read_image_array
       *                  write_image_array
       *                  get_image_size
       *                  allocate_image_array
       *                  free_image_array
       *
       *       Modifications:
       *          1 July 1999 - created
       *
       ***********************************************/


#include "cips.h"

int flip_image();
int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();


short **the_image;
short **out_image;

int main(argc, argv)
   int argc;
   char *argv[];
{

   char in_name[MAX_NAME_LENGTH], out_name[MAX_NAME_LENGTH];
   int  type;
   long length1, width1;

   if(argc != 4){
    printf("\n\nusage: flip in-file out-file type");
    printf("\n         type is 1 2 3 4 (90 degree increments)\n");
    exit(0);
   }

   strcpy(in_name,  argv[1]);
   strcpy(out_name, argv[2]);
   type = atoi(argv[3]);

      /* check input file */

   if(does_not_exist(in_name)){
      printf("\nERROR input file %s does not exist",
             in_name);
      exit(0);
   }  /* ends if does_not_exist */

   create_image_file(in_name, out_name);

      /* allocate the image arrays */

   get_image_size(in_name, &length1, &width1);
   the_image = allocate_image_array(length1, width1);
   out_image = allocate_image_array(length1, width1);

   read_image_array(in_name, the_image);

   flip_image(the_image, out_image, type, length1, width1);

   write_image_array(out_name, out_image);
   free_image_array(the_image, length1);
   free_image_array(out_image, length1);

}  /* ends main  */










int flip_image(the_image, out_image,
           type,
           rows, cols)
   int   type;
   long  cols, rows;
   short **the_image,
         **out_image;
{
   int    cd2, i, j, rd2;

     /*******************************************
     *
     *   Check the rotation_type.  If it is not
     *   a valid value, set it to 1.
     *
     *******************************************/

   if(type != 1  &&
      type != 2  &&
      type != 3  &&
      type != 4  &&
      type != 5) type = 1;

     /*******************************************
     *
     *   Rotate the image array as desired.
     *
     *******************************************/

     /*******************************************
     *
     *   1 90 degree rotation
     *
     *******************************************/

   if(type == 1  ||  type == 2  ||  type == 3){
      for(i=0; i<rows; i++){
         for(j=0; j<cols; j++)
            out_image[j][cols-1-i] = the_image[i][j];
      }  /* ends loop over i */
   }  /* ends if type == 1 or 2 or 3 */


     /*******************************************
     *
     *   a second 90 degree rotation
     *
     *******************************************/

   if(type == 2  ||  type == 3){
      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];
      for(i=0; i<rows; i++){
         for(j=0; j<cols; j++)
            out_image[j][cols-1-i] = the_image[i][j];
      }  /* ends loop over i */
   }  /* ends if type == 2 or 3 */


     /*******************************************
     *
     *   a third 90 degree rotation
     *
     *******************************************/

   if(type == 3){
      for(i=0; i<rows; i++)
         for(j=0; j<cols; j++)
            the_image[i][j] = out_image[i][j];
      for(i=0; i<rows; i++){
         for(j=0; j<cols; j++)
            out_image[j][cols-1-i] = the_image[i][j];
      }  /* ends loop over i */
   }  /* ends if type == 3 */


     /*******************************************
     *
     *   Flip the image array horizontally
     *   about the center vertical axis.
     *
     *******************************************/

   if(type == 4){
      cd2 = cols/2;
      for(j=0; j<cd2; j++){
         for(i=0; i<rows; i++){
            out_image[i][cols-1-j] = the_image[i][j];
         }  /* ends loop over i */
      }  /* ends loop over j */

      for(j=cd2; j<cols; j++){
         for(i=0; i<rows; i++){
            out_image[i][cols-1-j] = the_image[i][j];
         }  /* ends loop over i */
      }  /* ends loop over j */
   }  /* ends if type == 4 */


     /*******************************************
     *
     *   Flip the image array vertically
     *   about the center horizontal axis.
     *
     *******************************************/

   if(type == 5){
      rd2 = rows/2;
      for(i=0; i<rd2; i++){
         for(j=0; j<cols; j++){
            out_image[rows-1-i][j] = the_image[i][j];
         }  /* ends loop over j */
      }  /* ends loop over i */

      for(i=rd2; i<rows; i++){
         for(j=0; j<cols; j++){
            out_image[rows-1-i][j] = the_image[i][j];
         }  /* ends loop over j */
      }  /* ends loop over i */
   }  /* ends if type == 5 */

return(1);
}  /* ends flip_image */
