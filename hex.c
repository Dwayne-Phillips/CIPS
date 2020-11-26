

#include "cips.h"




int main(argc, argv)
   int argc;
   char *argv[];
{
   int i;
   char  buffer[10000];
   FILE *fp;

   fp = fopen(argv[1], "rb");
   fread(buffer, 10000, 1, fp);

   for(i=0; i<10000; i++){
      if( (i%2) == 0){
         printf("\n%4d>>%x %x",i, buffer[i], buffer[i+1]);
      }
   }


   fclose(fp);


}
