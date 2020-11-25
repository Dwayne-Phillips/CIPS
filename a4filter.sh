#!/bin/bash

# validated November 2020
# Make sure you are in the right directory

~/cips/mfilter a4.tif tmp.tif g H 1
~/cips/side a4.tif tmp.tif a4high1.tif Side-by-side
rm tmp.tif

~/cips/mfilter a4.tif tmp.tif g H 2
~/cips/side a4.tif tmp.tif a4high2.tif Side-by-side
rm tmp.tif

~/cips/mfilter a4.tif tmp.tif g H 3
~/cips/side a4.tif tmp.tif a4high3.tif Side-by-side
rm tmp.tif


~/cips/side a4high1.tif a4high2.tif tmp.tif Top-to-bottom
~/cips/side tmp.tif a4high2.tif a4HiFinal.tif Top-to-bottom
rm tmp.tif
rm a4high1.tif
rm a4high2.tif
rm a4high3.tif


~/cips/./mfilter a4.tif a4outh.tif h 3
~/cips/./mfilter a4.tif a4outl.tif l 3
~/cips/./mfilter a4.tif a4outm.tif m 3

~/cips/./side a4outl.tif a4outm.tif tmp.tif Side-by-side
~/cips/./side tmp.tif a4outh.tif tmp2.tif Side-by-side
~/cips/./side a4.tif tmp2.tif a4lmh.tif Side-by-side
rm tmp.tif
rm tmp2.tif
rm a4outh.tif
rm a4outl.tif
rm a4outm.tif



