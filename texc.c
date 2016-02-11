/*
   texc program

   usage:  texc input_file output_file

   Replace the TeX special characters with 
   the \ and special character.

   This is a helpful utility when converting a
   text file to a tex file.  The text files
   use many of the TeX special characters and 
   the \ needs to be put in front of the 
   special characters.

   27 February 2000
*/

#include <stdio.h>

main(argc, argv)
  int  argc;
  char *argv[];
{
  FILE *input_file;
  FILE *output_file;

  if(argc != 3){
    printf("\nusage: texc input_file output_file\n");
    exit (1);
  }

  if((input_file = fopen (argv[1], "rt")) == NULL){
    printf("\ntexc: error opening file %s\n", argv[1]);
    exit(1);
   }

  if((output_file = fopen (argv[2], "wt")) == NULL){
     printf("\ntexc: error opening file %s\n", argv[2]);
     exit(1);
  }

  insert_slash(input_file, output_file);
  fclose(input_file);
  fclose(output_file);
}  /* ends main */



insert_slash(input_file, output_file)
   FILE *input_file;
   FILE *output_file;
{
   char outs[300];
   char string[300];
   int  i, j, k;

   while(fgets(string, 300, input_file)){
         k = 0;
         for(i=0; string[i] != '\n' && string[i] != '\0'; i++){
              if(string[i] == '_'   ||
                 string[i] == '$'   ||
                 string[i] == '#'   ||
                 string[i] == '&'   ||
                 string[i] == '{'   ||
                 string[i] == '}'   ||
                 string[i] == '^'   ||
                 string[i] == '~'   ||
                 string[i] == '%'   ||  
                 string[i] == '\\'){
                   outs[k] = '\\';
                   k++;
                   outs[k] = string[i];
                   k++;
              }  /* ends if string[i] == a special character */
              else{
                    outs[k] = string[i];
                    k++;
              }  /* ends else */
         }  /* ends loop over i and string[i] */

         outs[k] = '\n';
	 outs[k+1] = '\0';
         fputs(outs, output_file);
         clear_out_buffer(string, 300);
         clear_out_buffer(outs,   300);
     }  /* ends while fgets */
}  /* ends insert_slash */


clear_out_buffer(string, n)
        char string[];
        int n;
{
     int i;
     for(i=0; i<n; i++)
            string[i] = ' ';
     string[0] = '\0';
}
