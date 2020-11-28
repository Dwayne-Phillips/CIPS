
# CIPS - The C Image Processing System

by Dwayne Phillips

28 November 2020

This is from my book "Image Processing in C"

which I wrote in 1994 and updated in 2000.

## The Book

A pdf of the book is in the file cips2ed.pdf.
This software illustrates basic image processing operations.
You can filter images, detect edges, and so on.
The book describes the concepts and points you to the source code files that implement the concepts.

## Fundamental Purpose

This is a collection of image processing operators and techniques.

The reading and writing of files is secondary.
The subroutines that perform the operations and techniques are designed to
work on an array of numbers.
That is all.

As stated later, this software has troubles reading many image files.
I apologize, but I wasn't concentrating on file formats.
Other programmers have done that work.

## Source Code

This GitHub repository contains all the source code mentioned in the book and a few more files.
It is all written in C.
I compiled and tested all the code in November 2020 on a Mac using the GNU tools (gcc).

The source code is at https://github.com/Dwayne-Phillips/CIPS.

## Making the Executables

This is all done in the style of creating programs on a Unix (Linux) system like we did in the 1980s
(yes, I am that old).
There is a makefile that contains the dependencies for each program.
Open a terminal window on a Mac or Linux system.
Type "make mainas" to compile and make the mainas program, which is the program that allows you to add and subtract images.

The rest work the same way.
Again, this is how we used to do it and this still works.


## TIF File Problems

My software that reads and writes TIF images is quite simple.
It doesn't work on "fancy" TIF files.
This means it works on TIF files I created.
It probably doesn't work on TIF files from other sources.
Sorry about this, but that was all I knew how to do.

If you want to use "fancy" TIFF files, I recommend finding and learning how to use
the libtiff source code.
Google "libtiff" and you will find all sorts of information.
One good source is http://www.simplesystems.org/libtiff/.

## TIF File Workaround

Since the source code only works with my simple TIF files and not complex ones,
I found a workaround to creating new TIF files. 
This involves using the GNU Image Processing software GIMP, which is found
at https://www.gimp.org/.

1. Grab any wonderful colorful jpg image from anywhere
2. Open that wonderful colorful jpg in GIMP
3. Convert the wonderful colorful jpg to a grayscale image in GIMP, Image --> mode --> grayscale
4. In GIMP, File --> Export the image as a bmp.

The CIPS software can read that bmp.

Finally, run the CIPS program bmp2tif to convert the bmp to a tif.




