

      /************************************************************
      *
      *   file d:\cips\cips.h
      *
      *       Functions: This file contains no functions. It
      *          contains declarations of the data structures used
      *          by the C Image Processing Systems CIPS.
      *
      *   Purpose:
      *      To declare data structures.
      *
      *   Modifications:
      *      June 1990 = created
      *
      *      February 2016 - in this postmodern world, 
      *      on a Mac with OS X 10.10...
      *      Longs are 8 bytes
      *      INTS are 4 bytes
      *      FLOATS are 4 bytes
      *
      *      You have to declare some functions before you use them.
      *      See end of this file.
      *
      *
      *
      **********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>


/****#include <graph.h>*****/
/***** #include <io.h> *****/
/**** #include <dos.h> ****/
/*** #include <malloc.h> ****/
/**** #include <sys\types.h> ****/
/**** #include <sys\stat.h> ****/

#define MAX_NAME_LENGTH       80
#define ROWS                 100
#define COLS                 100
#define GRAY_LEVELS          255
#define PREWITT                1
#define PEAK_SPACE            50
#define PEAKS                 30
#define KIRSCH                 2
#define SOBEL                  3
#define STACK_SIZE         40000
#define STACK_FILE_LENGTH    500
#define FORGET_IT            -50
#define STACK_FILE     "c:stack"


      /*********************************************
      *
      *   N O T E
      *   P O R T A B I L I T Y   I S S U E
      *
      *   The next two statements help port the 
      *   system to non-Microsoft C compilers.
      *   
      *   If you use Microsoft C, then you should
      *   #define MSC 1     and
      *   #undef  OTHERC
      *   
      *   If you are using another C compiler or
      *   are porting this to another type of 
      *   machine, then you should 
      *   #undef  MSC
      *   #define OTHERC 1
      *   
      *   Now you need to make changes in mymsc.c
      *
      **********************************************/

/*******#define MSC 1******/
/*******#undef  OTHERC******/
#define OTHERC 1
#undef  MSC

      /*********************************************
      *
      *   N O T E
      *   P O R T A B I L I T Y   I S S U E
      *
      *   Define the Microsoft contants.  If you
      *   are using OTHERC, then just put numbers
      *   there to hold a place.
      *
      *********************************************/

#ifdef MSC
#define VRES16COLOR  _VRES16COLOR
#define ERESCOLOR    _ERESCOLOR
#define MAXCOLORMODE _MAXCOLORMODE
#define HRESBW       _HRESBW
#define MRES4COLOR   _MRES4COLOR
#define TEXTC80      _TEXTC80
#define GCLEARSCREEN _GCLEARSCREEN
#endif

#ifdef OTHERC
#define VRES16COLOR  1
#define ERESCOLOR    1
#define MAXCOLORMODE 1
#define HRESBW       1
#define MRES4COLOR   1
#define TEXTC80      1
#define GCLEARSCREEN 1
#endif

      /**************************************************
      *
      *   The following struct defines the information
      *   you need to read from the tiff file
      *   header.
      *
      ***************************************************/

struct tiff_header_struct{
   short lsb;
   long  bits_per_pixel;
   long  image_length;
   long  image_width;
   long  strip_offset;
};

      /**************************************************
      *
      *   The following struct defines the information
      *   you need to read from the bmp file
      *   header.
      *
      ***************************************************/


struct bmpfileheader{
   unsigned short  filetype;
   unsigned long   filesize;
   short  reserved1;
   short  reserved2;
   unsigned long   bitmapoffset;
};

struct bitmapheader{
   unsigned long   size;
   long   width;
   long   height;
   unsigned short  planes;
   unsigned short  bitsperpixel;
   unsigned long   compression;
   unsigned long   sizeofbitmap;
   unsigned long   horzres;
   unsigned long   vertres;
   unsigned long   colorsused;
   unsigned long   colorsimp;
};

struct ctstruct{
   unsigned char blue;
   unsigned char green;
   unsigned char red;
};



                /*********************************************
                *
                *       The following four unions are used
                *       to put the bytes from the header
                *       into either an integer or a floating
                *       point number.
                *
                **********************************************/

union short_char_union{
        short s_num;
        char  s_alpha[2];
};

union int_char_union{
        int  i_num;
   char i_alpha[4];
};

union long_char_union{
        long  l_num;
        char  l_alpha[8];
};

union float_char_union{
        float f_num;
        char  f_alpha[4];
};

union ushort_char_union{
        short s_num;
        char  s_alpha[2];
};

union uint_char_union{
        int  i_num;
   char i_alpha[4];
};

union ulong_char_union{
        long  l_num;
        char  l_alpha[8];
};

/* for some reason, February 2016, you have to declare these two */
/* functions before you use them.                                */

short** allocate_image_array(long, long);
int free_image_array(short **, long);



