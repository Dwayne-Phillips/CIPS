/*

HERE"S HOW TO DO IT 20 Dec 99
DEFINITIONS:
Knowledge File - the whole thing
START TO END - a record
A PARAGRAPH - an entry

while(not end of file read){
  read a record
  write a record
} the end

read a record creates a list of lines.  that allows looking
ahead to the next line to see if the entry is done.
read a record will read from <START> to <END>
create a para code will format multiple line entries
correctly

   kf Program

   Dwayne Phillips
   December 1999

   Command Line:
      pageit in-file out-file -d -p -t title

   Purpose:
      This program reads my knowledge file (ASCII with
      format commands) and outputs the text to another
      ASCII file.

   Files:
      The program uses two files: (1) the input file,
      and (2) the output file.

      Input File
         The input file is unformatted ASCII text 
         created with vi or something simple like 
         that.  The lines can be as long or short 
         as you want (so long as they fit on the 
         screen of course).  

         The formatting commands are:
         <START> start of a record
         # any line starting with a pound sign is a comment
         #
         <T> title
         <A> authors
         <SBJ> subject, may have several subject lines
         <SRC> source
         <PAGES> pp. x-y
         <PY>  publication year
         <PM>  publication month
         <R> read date
         <E> emphasize this line
         .  level of indent 
         # a period at the start shows the level 
         # of the statement
         # one period is first level, two periods 
         # is second level etc.
         # 
         # If a line does not begin with a special character, 
         # it is a continuation
         # of the previous line.
         #
         <END> 

      Output File
         The output file is paginated ASCII text.  
         Instead of ragged text, the lines all fit 
         inside a formatted line.  Each page has a 
         header and footer.  The header may have a 
         heading title, date, and page number.  
         The user chooses to have or not have these 
         via the command line.

   Revision History:
      Version 1 - March 1996
      Version 2 - December 1999

*/
/**************************************************/
/**************************************************/
/*PAGE Includes and data structures */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define L              100
#define LPP             66
#define CPL             85
#define FOOTER           5
#define LEFT_MARGIN     10 
#define RIGHT_MARGIN     5 
#define END_OF_LIST   0x00
#define SPACE          ' '
#define VERSION    "kf Version 1 - December 1999"


struct word_list_struct{
   char   word[L];
   struct word_list_struct *next_word;
};

struct line_list_struct{
   char   line[L];
   struct line_list_struct *next_line;
};


   /* Here are the functions I needed to define. */

void   add_indents(char *, int);
struct word_list_struct * convert_lines_to_words(
                          struct line_list_struct *);
int    number_of_indent_levels(char *);
void   fill_page(FILE *, int *, int *);
void   fill_string(char *, int, char);
int    lpp(int, int);
void   output_line(char *, FILE *, int *, int *, 
                   char *, int, int, int);
void   print_report_header(FILE *, int *, int *, 
                   char *, int, int);
struct word_list_struct * read_a_paragraph(FILE *,
                                           int *);
struct line_list_struct * read_the_lines(FILE *, 
                                         int *);
struct line_list_struct * read_a_record(FILE *, 
                                        int *);
void   traverse_line_list(struct line_list_struct *);
void   traverse_word_list(struct word_list_struct *);
void   write_a_paragraph(struct word_list_struct *,
                         FILE *, int *, int *, 
                         char *, int, int, int);
void   write_a_record(struct line_list_struct *,
                      FILE *, int *, int *, 
                      char *, int, int, int);

void   strip_characters(char *, char *);

/**************************************************/
/**************************************************/

main(int argc, char *argv[])
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
   struct line_list_struct *line_list;

   if(argc < 2){
   printf(
   "\n\nusage: kf in-file out-file -d -p -n # -l # -t ..."
   "\n         -d     = put the date in the header"
   "\n         -p     = put the page number in the header"
   "\n         -l #   = there are # lines per page (%d default)"
   "\n         -n #   = start numbering the pages with #"
   "\n         -ds    = double space print the output"
   "\n         -t ... = put the following title in the header",
   LPP);
   printf(
   "\n%s", VERSION);
      exit(1);
   }

   if(argc > 1){
      strcpy(in_file_name,  argv[1]);
      strcpy(out_file_name, argv[2]);
      fill_string(title, L, '\0');

      i=3;
      while(i < argc){
         if(strcmp(argv[i], "-ds") == 0)
            double_space = 1;
         if(strcmp(argv[i], "-d") == 0)
            print_date = 1;
         if(strcmp(argv[i], "-p") == 0)
            print_page = 1;
         if(strcmp(argv[i], "-l") == 0){
            i++;
            lines = atoi(argv[i]);
         }
         if(strcmp(argv[i], "-n") == 0){
            i++;
            page_counter = atoi(argv[i]);
         }
         if(strcmp(argv[i], "-t") == 0){
            i++;
            strcpy(title, " ");
            while(i < argc){
               strcat(title, argv[i]);
               strcat(title, " ");
               i++;
            }
         }
         i++;
      }  /* ends loop over i argc */
   }  /* ends if argc > 1 */

      /* Set the static variable in the lpp routine */
      /* All other calls to lpp will read this by   */
      /* Calling with lpp(0,0);                     */
   lpp(lines, 1);


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

   print_report_header(out_file, &line_counter, 
                       &page_counter, title, 
                       print_date, print_page);
   while(file_done == 0){
      line_list = read_a_record(in_file, &file_done);
      write_a_record(line_list, out_file,
                        &line_counter,
                        &page_counter,
                        title,
                        print_date,
                        print_page,
                        double_space);
   }  /* ends while file_done is 0 */

   /***fill_page(out_file, &line_counter, &page_counter);***/

   fclose(in_file);
   fclose(out_file);
}  /* ends main */



/**************************************************/
/**************************************************/

/* 
   struct word_list_struct * convert_lines_to_words(...

*/

       
struct word_list_struct * convert_lines_to_words(
       struct line_list_struct *line_list)
{
   char   aword[L];

   int    copying    = 1,
          first_time = 1,
          i,
          j,
          last_line  = 0,
          line_index = 0,
          reading_lines = 1;

   struct line_list_struct *this_line;

   struct word_list_struct *new_word,
                           *result,
                           *temp;

   this_line = line_list;
   line_index = 0;

   while(reading_lines){


      if(this_line->next_line == END_OF_LIST){
         last_line = 1;
/*printf("\nTHIS LINE -%s-",this_line->line);*/
      }


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
               if(last_line)
                  reading_lines = 0;
            }  /* ends if aword is new line */
         }  /* ends else aword not a SPACE */

         i++;
         line_index++;

      }  /* ends while copying */
/*printf("\nA WORD is -%s-",aword);*/

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


   }  /* ends while reading_lines */

   return(result);

}  /* ends convert_lines_to_words */



/**************************************************/
/**************************************************/

/* 
   int lpp(...

   This routine returns the number of lines per page.
   This is my first use of a static variable.  
   The first call to this routine sets the static
   result to the value given.  All other calls to 
   the routine have the set parameter equal to zero.  
   In those cases the result does not change, it 
   remains what it was in the first call, and is 
   returned as such.
*/

int lpp(int lines_per_page, int set)
{
   static int result;
   if(set)
      result = lines_per_page;
   return(result);
}  /* ends lpp */
       



/**************************************************/
/**************************************************/

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
       



/**************************************************/
/**************************************************/

/* 
struct line_list_struct * read_the_lines(...
        FILE *, int *);

Here in kf, a paragraph ends when the first character
of a new line is a special character . < #
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
/*printf("\nREAD LINE %s\n",aline);*/
      }  /* ends if first_pass */

      else{  /* else not first_pass */
         new_one = (struct line_list_struct *) 
           calloc(1, sizeof(struct line_list_struct));
         temp->next_line    = new_one;
         temp               = new_one;
         new_one->next_line = END_OF_LIST;
         strcpy(new_one->line, aline);
      }  /* ends else not first_pass */

/*
     \
-------   here is where you change it
     /
   PROBLEM HERE
   This reading routine is setup so that it reads
   until it finds a blank line that is the quit signal.
   The quit signal comes after reading the lines.
   BUT WE want the quit signal (a new para marker) to
   come at the start of a new paragraph.  If I 
   read the first line of the new para I will lose it.
   So how do I know that the next para is starting
   without reading that line??????? 19 Dec 99
*/
      if(aline[0] == '\n'   ||
         aline[0] == '.'    ||
         aline[0] == '<'    ||
         aline[0] == '#'){
         reading = 0;
      }

   }  /* ends while reading */

   return(result);

}  /* ends read_the_lines */




/**************************************************/
/**************************************************/

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




/**************************************************/
/**************************************************/

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
                FILE *output_file, 
                int *line_counter, 
                int *page_counter, 
                char *title,
                int  print_date,
                int  print_page,
                int  double_space)
{
   char   line[L], line2[L];
   int    i,
          new_line = 1,
          spaces   = 0,
          too_long = 0,
          traversing_list = 1;
   struct word_list_struct *this_word;

char response[L];

/********************
printf("\nSTART OF write a para");
traverse_word_list(word_list);
spaces = number_of_indent_levels(word_list->word);
printf("\nSpaces=%d",spaces);
printf("\nPress enter to continue");
gets(response);
**********************/

   this_word = word_list;
   spaces    = number_of_indent_levels(word_list->word);
   fill_string(line, L, '\0');

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
/*printf("\nTHE LINE IS %s",line);*/
         strip_characters(line, line2);
         add_indents(line2, spaces);
         output_line(line2, output_file, 
                     line_counter, page_counter, 
                     title, print_date, print_page,
                     double_space);
      }  /* ends if 0 0 */

      if(traversing_list == 0 && too_long == 1){
         strcat(line, "\n");
         strip_characters(line, line2);
         add_indents(line2, spaces);
         output_line(line2, output_file, 
                     line_counter, page_counter, 
                     title, print_date, print_page,
                     double_space);
         fill_string(line, L, '\0');
         strcat(line, this_word->word);
         strcat(line, "\n");
         strip_characters(line, line2);
         add_indents(line2, spaces);
         output_line(line2, output_file, 
                     line_counter, page_counter, 
                     title, print_date, print_page,
                     double_space);
      }  /* ends 0 1 */

      if(traversing_list == 1 && too_long == 0){
         strcat(line, this_word->word);
/**********
printf("BUILDING LINE::%s",line);
gets(response);
**************/
         this_word = this_word->next_word;
      }  /* ends 1 0 */

      if(traversing_list == 1 && too_long == 1){
/************
printf("\nTRAVERSING AND LINE TOO LONG");      
printf("\nLINE IS:%s",line);
printf("\nLINE2 IS:%s",line2);
**************/
         strcat(line, "\n");
         strip_characters(line, line2);
         add_indents(line2, spaces);
         output_line(line2, output_file, 
                     line_counter, page_counter, 
                     title, print_date, print_page,
                     double_space);
         fill_string(line, L, '\0');
         strcat(line, this_word->word);
/**********
printf("OUTPUT WAS::%s",line2);
printf("\nStarted next line of this word list with:%s",line);
gets(response);
**************/
         this_word = this_word->next_word;
      }  /* ends 1 1 */

   }  /* ends while traversing_list */

}  /* ends write_a_paragraph */




/**************************************************/
/**************************************************/

/* 
   void output_line(...
*/

void output_line(char *line, 
                 FILE *output_file, 
                 int  *line_counter, 
                 int  *page_counter,
                 char *title, 
                 int  print_date,
                 int  print_page,
                 int  double_space)
{
   fputs(line, output_file);
   *line_counter = *line_counter + 1;
   if(*line_counter >= (lpp(0,0)-FOOTER)){
      fill_page(output_file,
                line_counter,
                page_counter);
      print_report_header(output_file,
                          line_counter,
                          page_counter,
                          title,
                          print_date,
                          print_page);
   }  /* ends if line_counter */

   if(double_space){
      fputs("\n", output_file);
      *line_counter = *line_counter + 1;
      if(*line_counter >= (lpp(0,0)-FOOTER)){
         fill_page(output_file,
                   line_counter,
                   page_counter);
         print_report_header(output_file,
                             line_counter,
                             page_counter,
                             title,
                             print_date,
                             print_page);
      }  /* ends if line_counter */
   }  /* ends if double_space */

}  /* ends output_line */




/**************************************************/
/**************************************************/

/* 
   void fill_page(...
*/

void fill_page(FILE *file_pointer,
               int *line_counter, 
               int *page_counter)
{
   char temp[L];
   int  i;

   strcpy(temp, "\n");

   while(*line_counter < (lpp(0,0)-FOOTER)){
      fputs(temp, file_pointer);
      *line_counter = *line_counter+1;
   }

   for(i=0; i<FOOTER-2; i++)
      fputs(temp, file_pointer);

   fputs("\n                    .",file_pointer);

   for(i=0; i<(FOOTER-(FOOTER-2)-1); i++)
      fputs(temp, file_pointer);

   *line_counter = 0;
   *page_counter = *page_counter + 1;

}  /* ends fill_page */




/**************************************************/
/**************************************************/

/* 
   void print_header(...
*/

void print_report_header(FILE *file_pointer,
                         int *line_counter,
                         int *page_counter,
                         char *title,
                         int  print_date,
                         int  print_page)
{
   char    blankline[L],
           classline[L],
           headerline[L],
           pageline[L],
           temp[L],
           time_string[L];
   int     i,
           l1,
           l2,
           l3,
           print_title = 1;
   struct tm *time_of_day;
   time_t seconds;

   time(&seconds);
   time_of_day = localtime(&seconds);
   sprintf(time_string, "%d-%d-%d\0",
           time_of_day->tm_mon+1,
           time_of_day->tm_mday,
           time_of_day->tm_year);

   if(title[0] == '\0')
      print_title = 0;
   fill_string(headerline, L, '\0');
   fill_string(pageline,   L, '\0');

   if(print_page){
      l3 = CPL-LEFT_MARGIN-RIGHT_MARGIN-RIGHT_MARGIN-9;
      pageline[0] = '\n';
      for(i=1; i<l3; i++)
         pageline[i] = SPACE;
      sprintf(temp, "%4d", *page_counter);
      strcat(pageline, "Page ");
      strcat(pageline, temp);
   }  /* ends if print_page */
   else
      strcpy(pageline, "\n");

   if(print_title == 0  &&  print_date == 0)
      strcpy(headerline, "\n");

   if(print_title == 0  &&  print_date == 1){
      l1 = strlen(time_string);
      l3 = CPL-LEFT_MARGIN-RIGHT_MARGIN-RIGHT_MARGIN-l1;
      headerline[0] = '\n';
      for(i=1; i<l3; i++)
         headerline[i] = SPACE;
      strcat(headerline, time_string);
   }  /* ends if print date but not title */

   if(print_title == 1  &&  print_date == 0){
      l1 = strlen(title);
      l3 = CPL-LEFT_MARGIN-RIGHT_MARGIN-RIGHT_MARGIN-l1;
      headerline[0] = '\n';
      for(i=1; i<l3; i++)
         headerline[i] = SPACE;
      strcat(headerline, title);
   }  /* ends if print title but not date */

   if(print_title == 1  &&  print_date == 1){
      strcpy(temp, title);
      strcat(temp, " - ");
      strcat(temp, time_string);
      l1 = strlen(temp);
      l3 = CPL-LEFT_MARGIN-RIGHT_MARGIN-RIGHT_MARGIN-l1;
      headerline[0] = '\n';
      for(i=1; i<l3; i++)
         headerline[i] = SPACE;
      strcat(headerline, temp);
   }  /* ends if print title and print date */

   strcpy(blankline, "\n");
   sprintf(classline, "\n");

   fputs(blankline,  file_pointer);
   fputs(classline,  file_pointer);
   fputs(headerline, file_pointer);
   fputs(pageline,   file_pointer);
   fputs(blankline,  file_pointer);

   *line_counter = 5;

}  /* ends print_report_header */


/**************************************************/
/**************************************************/

/* 
   void strip_characters(...
*/


void strip_characters(char *input, char *output)
{
   int i, j, not_yet;
   char response[L];

   fill_string(output, L, '\0');

   if(input[0] != '.'   &&
      input[0] != '<'){
      for(i=0; i<L; i++)
         output[i] = input[i];
   }  /* ends if no special characters */

   if(input[0] == '.'){
      j = 0;
      for(i=0; i<L; i++){
         if(input[i] == '.'){
            /* do nothing */
         }
         else{
            output[j++] = input[i];
         }
      }  /* ends loop over i */
   }  /* period case */
   
   if(input[0] == '<'){
      not_yet = 1;
      j = 0;
      for(i=0; i<L; i++){
         if(not_yet == 0){
            output[j++] = input[i];
         }
         if(input[i] == '>'){
            not_yet = 0;
         }
      }  /* ends loop over i */
   }  /* < case */

}  /* ends strip_characters */
       



/***********************************************/
/***********************************************/

/* 
struct line_list_struct * read_a_record(...
        FILE *, int *);

Read until hitting <START>.
Do not put the <START> line in the list of lines.
Put all the lines in the line list until 
   hitting <END>
Do not put the <END> line in the list of lines.
*/

struct line_list_struct * read_a_record(
        FILE *input_file, int *file_done)
{
   char   aline[L];
   int    first_pass = 1,
          searching  = 1,
          reading    = 1;
   struct line_list_struct *new_one,
                           *result,
                           *temp;

   while(searching){
      if(fgets(aline, L, input_file) == NULL){
         *file_done = 1;
         reading    = 0;
         searching = 0;
      }  /* ends if fgets is NULL */
      else{
         if(strncmp(aline, "<START>", 7) == 0)
            searching = 0;
      }  /* ends else */
   }  /* ends while searching */


   while(reading){

      if(fgets(aline, L, input_file) == NULL){
         *file_done = 1;
         reading    = 0;
      }  /* ends if fgets is NULL */

      if(strncmp(aline, "<END>", 5) == 0)
            reading = 0;
      else{ /* not END so process line */

         if(first_pass){
            new_one = (struct line_list_struct *) 
              calloc(1, sizeof(struct line_list_struct));
            temp               = new_one;
            result             = new_one;
            first_pass         = 0;
            new_one->next_line = END_OF_LIST;
            strcpy(new_one->line, aline);
/*printf("\nput this line in the list\n%s\n", aline);*/
         }  /* ends if first_pass */
   
         else{  /* else not first_pass */
            new_one = (struct line_list_struct *) 
              calloc(1, sizeof(struct line_list_struct));
            temp->next_line    = new_one;
            temp               = new_one;
            new_one->next_line = END_OF_LIST;
            strcpy(new_one->line, aline);
/*printf("\nput this line in the list\n%s\n", aline);*/
         }  /* ends else not first_pass */

      }  /* ends else not END */
   
   }  /* ends while reading */

   return(result);

}  /* ends read_a_record */




/***********************************************/
/***********************************************/

/* 

void   traverse_line_list(struct line_list_struct *)

*/

void   traverse_line_list(struct line_list_struct *this_list)
{
   struct line_list_struct *temp;

   temp = this_list;
   while(temp != END_OF_LIST){
      printf("--%s--", temp->line);
      temp = temp->next_line;
   }  /* ends while */
}  /* ends traverse_line_list */




/***********************************************/
/***********************************************/

/* 

void   traverse_word_list(struct word_list_struct *)

*/

void   traverse_word_list(struct word_list_struct *this_list)
{
   struct word_list_struct *temp;

   temp = this_list;
   while(temp != END_OF_LIST){
      printf("--%s", temp->word);
      temp = temp->next_word;
   }  /* ends while */
}  /* ends traverse_word_list */




/**************************************************/
/**************************************************/

/* 
   void write_a_record(...

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

void write_a_record(
                struct line_list_struct *line_list,
                FILE *out_file, 
                int *line_counter, 
                int *page_counter, 
                char *title,
                int  print_date,
                int  print_page,
                int  double_space)
{
   char   line[L], line2[L];
   char   response[L];
   int    first_line_of_entry = 1,
          i               = 0,
          new_line        = 1,
          this            = 0,
          too_long        = 0,
          traversing_list = 1;

   struct line_list_struct *current_line,
                           *end_of_current,
                           *temp,
                           *the_next_line;
   struct word_list_struct *words;

   temp          = line_list;
   the_next_line = line_list;

   while(temp != END_OF_LIST){
      
      /****************************
      if(temp->line[0] == '\n'   ||
         temp->line[0] == ' '    ||
         temp->line[0] == '#')
         this = BLANK_OR_COMMENT;
      *****************************/

      if(the_next_line->line[0] == '<' ||
         the_next_line->line[0] == '.'){

            /* new entry AND first line of entry
               so put the entry into the current
               line */
         if(first_line_of_entry){
            first_line_of_entry = 0;
            current_line   = temp;
            end_of_current = temp;
            temp           = temp->next_line;
            the_next_line  = temp;
         }  /* ends if first_line_of_entry */

            /* new entry AND not first line of entry
               so stop adding to the current line,
               convert it to a word list, convert
               the word list to a paragraph,
               and print the paragraph */
         else{ /* not first_line_of_entry */
            first_line_of_entry = 1;
            end_of_current->next_line = END_OF_LIST;
            words = convert_lines_to_words(current_line);
            write_a_paragraph(words, out_file,
                              line_counter,
                              page_counter,
                              title,
                              print_date,
                              print_page,
                              double_space);
            current_line = the_next_line;
         }  /* ends else not first_line_of_entry */

      }  /* ends if a new entry */

         /* NOT a new entry AND don't care
            so add the next line to the current
            line */
      else{  /* not a new entry */
         end_of_current = the_next_line;
         temp           = temp->next_line;
         the_next_line  = temp;
      }  /* ends else not a new entry */

   }  /* ends while */

      /* take care of the last current line */
   if(current_line->line[0] == '<' ||
      current_line->line[0] == '.'){
         words = convert_lines_to_words(current_line);
         current_line = the_next_line;
         write_a_paragraph(words, out_file,
                           line_counter,
                           page_counter,
                           title,
                           print_date,
                           print_page,
                           double_space);
   }  /* ends if current_line is an entry */

      /* put a few blank lines after each record */
   fill_string(line, L, '\0');
   strcat(line, "\n");
   output_line(line, out_file, 
               line_counter, page_counter, 
               title, print_date, print_page,
               double_space);
   output_line(line, out_file, 
               line_counter, page_counter, 
               title, print_date, print_page,
               double_space);
   output_line(line, out_file, 
               line_counter, page_counter, 
               title, print_date, print_page,
               double_space);


}  /* ends write_a_record */



/**************************************************/
/**************************************************/

int number_of_indent_levels(char *line)
{
   int i, 
       result = 0;
 
   for(i=0; i<L; i++){
      if(line[i] == '.')
         result++;
      if(line[i] != '.')
         i = L+1;
   }  /* ends loop over i */

   return(result);
}  /* ends number_of_indent_levels */



/**************************************************/
/**************************************************/

void add_indents(char *line, int spaces)
{
   int i, 
       result = 0;
   char temp[L];

   fill_string(temp, L, '\0');
   for(i=0; i<spaces*2; i++)
      temp[i] = SPACE;

   strcat(temp, line);

   for(i=0; i<L; i++)
      line[i] = temp[i];
 
}  /* ends add_indents */
