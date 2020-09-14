



/********************************************************/
/********************************************************/

/*
          This program prepares an ASCII file for
          input into MS Word.  It removes the hard
          carraige returns from the end of each line.
          It keeps the return when there are two in
          a row.  That makes the paragraphs correct
          for MS Word.
*/
/********************************************************/

/******************* includes ***************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



/******************* defines ****************************/
#define  MIN_PARAMS 3
#define  MAX_PARAMS 3
#define  VERSION "DPFILT Version 1.0 - 3 November 1996"
#define  B 30000



/**************** function declarations *****************/
void print_usage(void);



/********************************************************/
/*
   main

*/

char in_buffer[B], out_buffer[B];


int main (int argc, char *argv[])
{
   size_t ip = 0, op = 0;
   FILE *input_file, *output_file;
   size_t bytes_read = 0;

   printf("\n\n%s\n\n", VERSION);

   if(argc < MIN_PARAMS  ||  argc > MAX_PARAMS){
      print_usage();
      exit(0);
   }  /* ends if argc */

   if(strcmp(argv[1], argv[2]) == 0){
      printf(
       "\nERROR: Input and output file names must be different");
      print_usage();
      exit(0);
   } /* ends if strcmp */

   if((input_file = fopen(argv[1], "r")) == NULL){
      printf("\nERROR: Cannot open file %s", argv[1]);
      exit(1);
   }  /* ends if input_file */

   if((output_file = fopen(argv[2], "w")) == NULL){
      printf("\nERROR: Cannot create file %s", argv[2]);
      fclose(input_file);
      exit(1);
   }  /* ends if output_file */


   bytes_read = B;
   while(bytes_read == B){
      ip = 0;
      op = 0;
      bytes_read = fread(in_buffer, sizeof(char), B, input_file);
      printf("\nRead %ld (%d)", bytes_read, B);
      while(ip < bytes_read){
         if(in_buffer[ip] == '\n'){
            if(in_buffer[ip+1] == '\n'){
               out_buffer[op] = in_buffer[ip];
               op = op+1;
               ip = ip+2;
            }
            else{
               out_buffer[op] = ' ';
               ip++;
               op++;
            }
         }
         else{
            out_buffer[op++] = in_buffer[ip++];
         }
      }  /* ends while */
      fwrite(out_buffer, sizeof(char), op, output_file);
   }  /* ends while */

   fclose(input_file);
   fclose(output_file);

   printf("\n\n%s\n\n", VERSION);

}  /* ends main */





/********************************************************/
/*
   print_usage()
*/

void print_usage()
{
   printf("\nusage: dpfilt in-file out-file");
   printf("\n"
          "\nThis program prepares an ASCII file for"
          "\ninput into MS Word.  It removes the hard"
          "\ncarraige returns from the end of each line."
          "\nIt keeps the return when there are two in"
          "\na row.  That makes the paragraphs correct"
          "\nfor MS Word.");
}  /* ends print_usage */


