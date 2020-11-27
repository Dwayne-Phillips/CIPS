
# CIPS - The C Image Processing System

by Dwayne Phillips

This is from my book "Image Processing in C"

which I wrote in 1994 and updated in 2000.

## The Book

A pdf of the book is in the file cips2ed.pdf.

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

