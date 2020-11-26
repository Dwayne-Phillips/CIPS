
/*

   Para Program

   Dwayne Phillips
   March 1996

   Command Line:
      para in-file out-file

   Purpose:
      This program does a simple form a text formatting.
      It takes in an ASCII text file and formats it.
      The input is just text.  Paragraphs are delimited
      by having a blank line between them.

      This program borrows from the pageit program.
      Pageit formatted text to be printed.

      Para formats text for another file with no page
      breaks, headers, footers, etc.

   Files:
      The program uses two files: (1) the input file,
      and (2) the output file.

      Input File
         The input file is unformatted ASCII text
         created with vi or something simple like
         that.  The lines can be as long or short
         as you want (so long as they fit on the
         screen of course).  The separate paragraphs,
         leave a blank line.

      Output File
         The output file is also ASCII text.
         Instead of ragged text, the lines all fit
         There are no pages.  The text flows from
         para to para with a blank line between.

   Revision History:
      Version 1 - January 1999

*/
/*****************************************************/
/*****************************************************/
/*PAGE Includes and data structures */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
/** #include <malloc.h> **/
#include <string.h>

#define L              100
#define LPP             66
#define CPL             85
#define FOOTER           5
#define LEFT_MARGIN      0
#define RIGHT_MARGIN    20
#define END_OF_LIST   0x00
#define SPACE          ' '
#define VERSION    "Para Version 1 - January 1999"


struct word_list_struct{
   char   word[L];
   struct word_list_struct *next_word;
};

struct line_list_struct{
   char   line[L];
   struct line_list_struct *next_line;
};


   /* Here are the functions I needed to define. */

struct word_list_struct * convert_lines_to_words(
                          struct line_list_struct *);
void   fill_string(char *, int, char);
void   output_line(char *, FILE *);
struct word_list_struct * read_a_paragraph(FILE *,
                                           int *);
struct line_list_struct * read_the_lines(FILE *,
                                         int *);
void   write_a_paragraph(struct word_list_struct *,
                         FILE *);

/*****************************************************/
/*****************************************************/

int main(int argc, char *argv[])
{

   char  in_file_name[L],
         out_file_name[L],
         title[L];

   FILE  *in_file, *out_file;

   int   double_space = 0,
         file_done    = 0,
         i,
         line_counter = 0,
         lines        = LPP,
         page_counter = 1,
         print_date   = 0,
         print_page   = 0,
         print_title  = 0;

   struct word_list_struct *word_list, *temp;

   if(argc != 3){
   printf("\n\nusage: para in-file out-file ");
   printf(
   "\n%s\n", VERSION);
      exit(1);
   }

   if(argc == 3){
      strcpy(in_file_name,  argv[1]);
      strcpy(out_file_name, argv[2]);
   }  /* ends if argc == 3 */



   if((in_file = fopen(in_file_name, "rt")) == NULL){
      printf("\nERROR Could not open file %s",
             in_file_name);
      exit(2);
   }

   if((out_file = fopen(out_file_name, "wt")) == NULL){
      printf("\nERROR Could not open file %s",
             out_file_name);
      exit(2);
   }

   while(file_done == 0){
      word_list = read_a_paragraph(in_file, &file_done);
      write_a_paragraph(word_list, out_file);
   }  /* ends while file_done is 0 */

   fclose(in_file);
   fclose(out_file);
}  /* ends main */



/*****************************************************/
/*****************************************************/

/*
   struct word_list_struct * convert_lines_to_words(...

*/

struct word_list_struct * convert_lines_to_words(
       struct line_list_struct *line_list)
{
   char   aword[L];

   int    copying    = 1,
          doit       = 1,
          first_time = 1,
          i,
          j,
          line_index = 0;

   struct line_list_struct *this_line;

   struct word_list_struct *new_word,
                           *result,
                           *temp;

   this_line = line_list;
   line_index = 0;

   while(doit){
      if(this_line->next_line == END_OF_LIST)
         doit = 0;
         /* still need to do this line of text */
      copying    = 1;
      i          = 0;
      fill_string(aword, L, '\0');
      while(copying){
         aword[i] = this_line->line[line_index];

         if(aword[i] == SPACE){

            copying  = 0;
            if(this_line->line[line_index+1] == SPACE){
               i++;
               line_index++;
               aword[i] = this_line->line[line_index];
            }  /* ends if line is SPACE */
         }  /* ends if aword is SPACE */

         else{  /* else aword not a SPACE */
            if(aword[i] == '\n'){
               aword[i] = SPACE;
               copying  = 0;
               this_line = this_line->next_line;
               line_index = -1;
            }  /* ends if aword is new line */
         }  /* ends else aword not a SPACE */

         i++;
         line_index++;

      }  /* ends while copying */

      if(first_time){
         first_time = 0;
         new_word = (struct word_list_struct *)
            calloc(1, sizeof(struct word_list_struct));
         strcpy(new_word->word, aword);
         new_word->next_word = END_OF_LIST;
         result = new_word;
         temp   = new_word;
      }  /* ends if first_time */

      else{  /* else not first_time */
         new_word = (struct word_list_struct *)
            calloc(1, sizeof(struct word_list_struct));
         strcpy(new_word->word, aword);
         new_word->next_word = END_OF_LIST;
         temp->next_word = new_word;
         temp = new_word;
      }  /* ends else not first_time */


   }  /* ends while doit */

   return(result);

}  /* ends convert_lines_to_words */




/*****************************************************/
/*****************************************************/





/*****************************************************/
/*****************************************************/

/*
   void fill_string(...
*/

void fill_string(char *string, int size,
                 char fill_char)
{
   int i;
   for(i=0; i<size; i++)
      string[i] = fill_char;
}  /* ends fill_string */




/*****************************************************/
/*****************************************************/

/*
struct line_list_struct * read_the_lines(...
        FILE *, int *);
*/

struct line_list_struct * read_the_lines(
        FILE *input_file, int *file_done)
{
   char   aline[L];
   int    first_pass = 1,
          reading    = 1;
   struct line_list_struct *new_one,
                           *result,
                           *temp;

   while(reading){

      if(fgets(aline, L, input_file) == NULL){
         *file_done = 1;
         reading    = 0;
      }  /* ends if fgets is NULL */

      if(first_pass){
         new_one = (struct line_list_struct *)
           calloc(1, sizeof(struct line_list_struct));
         temp               = new_one;
         result             = new_one;
         first_pass         = 0;
         new_one->next_line = END_OF_LIST;
         strcpy(new_one->line, aline);
      }  /* ends if first_pass */

      else{  /* else not first_pass */
         new_one = (struct line_list_struct *)
           calloc(1, sizeof(struct line_list_struct));
         temp->next_line    = new_one;
         temp               = new_one;
         new_one->next_line = END_OF_LIST;
         strcpy(new_one->line, aline);
      }  /* ends else not first_pass */

      if(aline[0] == '\n'){
         reading = 0;
      }

   }  /* ends while reading */

   return(result);

}  /* ends read_the_lines */




/*****************************************************/
/*****************************************************/

/*
   struct word_list_struct * read_a_paragraph(...
*/


struct word_list_struct * read_a_paragraph(
                            FILE *input_file,
                            int  *file_done)
{
   struct line_list_struct *line_list;
   struct word_list_struct *result;

   line_list = read_the_lines(input_file, file_done);
   result    = convert_lines_to_words(line_list);
   free(line_list);
   return(result);
}  /* ends read_a_paragraph */




/*****************************************************/
/*****************************************************/

/*
   void write_a_paragraph(...

   traversing_list == 0 means this is the last word
                        in the paragraph
   too_long == 1 means this word will not fit on the
                 current line

   traversing_list  too_long
   0  0  put word on line
         print line
         THE END

   0  1  print line
         start new line
         put word on line
         print line
         THE END

   1  0  put word on line
         go on

   1  1  print line
         start new line
         put word on new line
         go on

*/

void write_a_paragraph(
                struct word_list_struct *word_list,
                FILE *output_file)
{
   char   line[L];
   int    i,
          new_line = 1,
          too_long = 0,
          traversing_list = 1;
   struct word_list_struct *this_word;

   this_word = word_list;
   fill_string(line, L, '\0');
   fill_string(line, LEFT_MARGIN, SPACE);

   while(traversing_list){
         /* this is the last word in the paragraph */
      if(this_word->next_word == END_OF_LIST)
         traversing_list = 0;

         /* If this word won't fit on this line, */
      if( (strlen(this_word->word) + (strlen(line))) >
          (CPL - (RIGHT_MARGIN+LEFT_MARGIN)) )
         too_long = 1;
      else
         too_long = 0;

      if(traversing_list == 0 && too_long == 0){
         strcat(line, this_word->word);
         strcat(line, "\n");
         output_line(line, output_file);
      }  /* ends if 0 0 */

      if(traversing_list == 0 && too_long == 1){
         strcat(line, "\n");
         output_line(line, output_file);
         fill_string(line, L, '\0');
         fill_string(line, LEFT_MARGIN, SPACE);
         strcat(line, this_word->word);
         strcat(line, "\n");
         output_line(line, output_file);
      }  /* ends 0 1 */

      if(traversing_list == 1 && too_long == 0){
         strcat(line, this_word->word);
         this_word = this_word->next_word;
      }  /* ends 1 0 */

      if(traversing_list == 1 && too_long == 1){
         strcat(line, "\n");
         output_line(line, output_file);
         fill_string(line, L, '\0');
         fill_string(line, LEFT_MARGIN, SPACE);
         strcat(line, this_word->word);
         this_word = this_word->next_word;
      }  /* ends 1 1 */

   }  /* ends while traversing_list */

   fill_string(line, L, '\0');
   fill_string(line, LEFT_MARGIN, SPACE);
   strcat(line, "\n");
   output_line(line, output_file);

}  /* ends write_a_paragraph */




/*****************************************************/
/*****************************************************/

/*
   void output_line(...
*/

void output_line(char *line,
                 FILE *output_file)
{
   fputs(line, output_file);
}  /* ends output_line */




/*****************************************************/
/*****************************************************/
