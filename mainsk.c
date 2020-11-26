
       /*********************************************
       *
       *   file mainsk.c
       *
       *   Functions: This file contains
       *      main
       *      show_mainsk_usage
       *
       *   Purpose:
       *      This file contains the main calling
       *      routine that calls the erosion,
       *      dilation, outline, and skeleton
       *      functions.
       *
       *   External Calls:
       *      imageio.c - create_image_file
       *                  read_image_array
       *                  write_image_array
       *                  get_image_size
       *                  allocate_image_array
       *                  free_image_array
       *      ed.c - erosion
       *             dilation
       *             mask_erosion
       *             mask_dilation
       *             interior_outline
       *             exterior_outline
       *             opening
       *             closing
       *      skeleton.c - thinning
       *                   skeleton
       *                   dilate_not_join
       *                   special_opening
       *                   special_closing
       *                   edm
       *                   mat
       *
       *   Modifications:
       *      7 March 1993 - created
       *      21 August 1998 - modified to work on entire
       *           images at once.
       *      19 September 1998 - modified to work with
       *            all I O routines in imageio.c.
       *
       ***********************************************/

#include "cips.h"

int does_not_exist();
int create_image_file();
int get_image_size();
int read_image_array();
int free_image_array();
int write_image_array();
int thinning();
int dilate_not_join();
int erosion();
int dilation();
int mask_erosion();
int mask_dilation();
int interior_outline();
int exterior_outline();
int opening();
int closing();
int special_closing();
int special_opening();
int edm();
int mat();
int show_mainsk_usage();

int main(argc, argv)
   int argc;
   char *argv[];
{

   char     name1[80], name2[80], type[80];
   int      i, j, mask_type,
            number, threshold;
   long     length, width;
   short    value;
   short    **the_image;
   short    **out_image;


       /****************************************
       *
       *   Interpret the command line parameters.
       *
       *****************************************/

   if(argc < 5){
      show_mainsk_usage();
      exit(0);
   }

   strcpy(name1,    argv[1]);
   strcpy(name2,    argv[2]);
   strcpy(type,     argv[3]);
   value     = atoi(argv[4]);

   if(does_not_exist(name1)){
    printf("\nERROR input file %s does not exist",
             name1);
    exit(0);
   }

   if(argc >= 5){
      threshold = atoi(argv[5]);
      mask_type = atoi(argv[5]);
   }

   if(argc >= 6)
      number = atoi(argv[6]);


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

   for(i=0; i<length; i++)
      for(j=0; j<width; j++)
         out_image[i][j] = 0;

       /*********************************************
       *
       *   Call the desired function.
       *
       **********************************************/


            /* thinning */
   if(strncmp("thi", type, 3) == 0){
      thinning(the_image, out_image,
               value, threshold, 0,
               length,
               width);
   }  /* ends thinning operation */

            /* dilate-not-join */
   if(strncmp("dnj", type, 3) == 0){
      dilate_not_join(the_image, out_image,
                      value, threshold,
                      length,
                      width);
   }  /* ends dilate_not_join operation */

            /* erosion */
   if(strncmp("ero", type, 3) == 0){
      erosion(the_image, out_image,
              value, threshold,
              length,
              width);
   }  /* ends erosion operation */

            /* dilation */
   if(strncmp("dil", type, 3) == 0){
      dilation(the_image, out_image,
               value, threshold,
               length,
               width);
   }  /* ends dilation operation */

            /* mask_erosion */
   if(strncmp("mer", type, 3) == 0){
      mask_erosion(the_image, out_image,
                   value, mask_type,
                   length,
                   width);
   }  /* ends mask_erosion operation */

            /* mask_dilation */
   if(strncmp("mdi", type, 3) == 0){
      mask_dilation(the_image, out_image,
                    value, mask_type,
                    length,
                    width);
   }  /* ends mask_dilation operation */

            /* interior_outline */
   if(strncmp("int", type, 3) == 0){
      interior_outline(the_image, out_image,
                       value, mask_type,
                       length,
                       width);
   }  /* ends interior_outline operation */

            /* exterior_outline */
   if(strncmp("ext", type, 3) == 0){
      exterior_outline(the_image, out_image,
                       value, mask_type,
                       length,
                       width);
   }  /* ends exterior_outline operation */

            /* opening */
   if(strncmp("ope", type, 3) == 0){
      opening(the_image, out_image,
              value, mask_type, number,
              length,
              width);
   }  /* ends opening operation */

            /* closing */
   if(strncmp("clo", type, 3) == 0){
      closing(the_image, out_image,
              value, mask_type, number,
              length,
              width);
   }  /* ends closing operation */

            /* special opening */
   if(strncmp("spo", type, 3) == 0){
      special_opening(the_image, out_image,
                      value, threshold, number,
                      length,
                      width);
   }  /* ends special opening operation */

            /* special closing */
   if(strncmp("spc", type, 3) == 0){
      special_closing(the_image, out_image,
                      value, threshold, number,
                      length,
                      width);
   }  /* ends special closing operation */

            /* Euclidean Distance Measure */
   if(strncmp("edm", type, 3) == 0){
      edm(the_image, out_image,
          value,
          length,
          width);
   }  /* ends Euclidean distance mesaure */

            /* medial axis transform */
   if(strncmp("mat", type, 3) == 0){
      mat(the_image, out_image,
          value,
          length,
          width);
   }  /* ends medial axis transform operation */

   write_image_array(name2, out_image);

   free_image_array(out_image, length);
   free_image_array(the_image, length);

}  /* ends main  */






int show_mainsk_usage()
{
   char response[80];

printf(
"\n\nNot enough parameters:"
"\n"
"\n usage: mainsk in-file out-file type value"
" [threshold-or-mask-type] [number]"
"\n"
"\n recall type: EROsion DILation Mask-ERosion"
"\n              Mask_DIlation INTerior-outline"
"\n              EXTerior-outline THInning"
"\n              Dilate-Not-Join OPEning"
"\n              CLOsing SPecial-Opening"
"\n              SPecial-Closing"
"\n              Euclidean-Distance-Measure"
"\n              Medial-Axis-Transform");

printf("\nPress Enter to see more");
gets(response);

printf("\n"
"\n   MASK DILATION"
"\nmainsk in-file out-file mdi value mask-type"
"\n   MASK EROSION"
"\nmainsk in-file out-file mer value mask-type"
"\n   EROSION "
"\nmainsk in-file out-file ero value threshold"
"\n   DILATION "
"\nmainsk in-file out-file dil value threshold"
"\n   INTERIOR OUTLINE "
"\nmainsk in-file out-file int value mask-type"
"\n   EXTERIOR OUTLINE "
"\nmainsk in-file out-file ext value mask-type"
"\n   OPENING"
"\nmainsk in-file out-file ope value mask-type number"
"\n   CLOSING"
"\nmainsk in-file out-file clo value mask-type number");

printf("\n   THINNING"
"\nmainsk in-file out-file thi value threshold"
"\n   DILATE NOT JOIN"
"\nmainsk in-file out-file dnj value threshold"
"\n   SPECIAL CLOSING"
"\nmainsk in-file out-file spc value threshold number"
"\n   SPECIAL OPENING"
"\nmainsk in-file out-file spo value threshold number"
"\n   EUCLIDEAN DISTANCE MEASURE"
"\nmainsk in-file out-file edm value"
"\n   MEDIAL AXIS TRANSFORM"
"\nmainsk in-file out-file mat value"
"\n "
"\n value is usually 1"
"\n mask-type is 1-4 inclusive"
"\n threshold is 0-8 inclusive"
"\n number is number of erosions or dilations"
"\n");

return(1);
}
