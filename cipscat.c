


       /*********************************************
       *
       *   file d:\cips\cipscat.c
       *
       *   Functions: This file contains
       *     main
       *
       *   Purpose:
       *     This file contains a program that
       *     concatenates .c files together, but
       *     only copies the first occurence of the
       *     include cips.h statement.
       *
       *     This is very helpful when you need to
       *     concatenate CIPS code files together
       *     for compiling.
       *
       *     For example, the cips3.c file contains
       *     the files addsub.c cutp.c and rotate.c
       *     To combine these for compiling you run
       *
       *     cipscat addsub.c cutp.c rotate.c -o cips3.c
       *
       *   External Calls:
       *      none
       *
       *   Modifications:
       *      12 May 1993 - created
       *
       ************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  LENGTH 100

int main(int argc, char *argv[])
{
   char input_file_name[LENGTH],
        output_file_name[LENGTH],
        string[LENGTH],
        string2[LENGTH];

   FILE *input_file;
   FILE *output_file;

   int  i, j=0;

      /*****************************************
      *
      *   Look at the command line to ensure
      *   it is correct.
      *
      *****************************************/

   if(argc < 4){
      printf("\nusage: cipscat in1 in2 ... inx "
             "-o out");
      printf("\n\n");
      exit(1);
   }

   if(strcmp("-o", argv[argc-2]) != 0){
      printf("\nusage: cipscat in1 in2 ... inx "
             "-o out");
      exit(1);
   }

      /*****************************************
      *
      *   Ensure none of the input file names
      *   match the output file name.
      *
      *****************************************/

   for(i=1; i<(argc-2); i++){
      if(strcmp(argv[i], argv[argc-1]) == 0){
         printf("\nERROR: input file %s has the "
                "same name as output file %s\n",
                argv[i], argv[argc-1]);
         exit(2);
      }
   }

      /*****************************************
      *
      *   Open the output file.
      *   Put a header comment in it.
      *
      *****************************************/

   strcpy(output_file_name, argv[argc-1]);
   if((output_file = fopen(output_file_name, "wt"))
                             == NULL){
      printf("\ncipscat: Error file %s\n",
              output_file_name);
      exit(1);
   }

   fputc('\n', output_file);
   sprintf(string,
           "   /*************************** \n");
   fputs(string, output_file);
   sprintf(string,
           "   * \n");
   fputs(string, output_file);
   sprintf(string, "   *   %s \n",
           output_file_name);
   fputs(string, output_file);
   sprintf(string,
      "   *   COMPOSITE FILE COMPRISING: \n");
   fputs(string, output_file);

   for(i=1; i<(argc-2); i++){
      sprintf(string, "   *   %s \n",
              argv[i]);
      fputs(string, output_file);
   }
   sprintf(string,
           "   * \n");
   fputs(string, output_file);
   sprintf(string,
           "   ***************************\\ \n");
   fputs(string, output_file);
   fputc('\n', output_file);
   fputc('\n', output_file);


      /*****************************************
      *
      *   Loop through the input files.
      *   Copy all of the first one to the
      *   output.  For the rest, look for the
      *   include cips.h statement and do not copy
      *   that to the output.
      *
      *****************************************/

   for(i=1; i<(argc-2); i++){

      strcpy(input_file_name, argv[i]);
      if((input_file = fopen(input_file_name, "rt"))
                                == NULL){
         printf("\ncipscat: Error file %s\n",
                 input_file_name);
         exit(2);
      }  /* ends if fopen input_file */

      printf("\n\tcopying %s to %s",
       input_file_name, output_file_name);

      if(i==1){ /* First file, so copy it all */
         while(fgets(string, LENGTH, input_file))
            fputs(string, output_file);
      }  /* ends if first input file */

      else{ /* Other files, look for include */
         while(fgets(string, LENGTH, input_file)){
            if(strncmp(
             string, "#include \"cips.h\"", 15) == 0)
               j++;
            else
               fputs(string, output_file);
         }
      }  /* ends else all the other input files */

      fclose(input_file);
   }  /* ends the main loop over i */

   fclose(output_file);
   printf("\n");
   return(0);
}  /* ends main */
