#!/bin/bash

# validated November 2020
# you have to make sure you are in the right directory

~/cips/himage a4.tif a4hist.tif 400 400
~/cips/side a4.tif a4hist.tif a4side.tif Side-by-side

~/cips/histeq a4.tif a4eq.tif
~/cips/himage a4eq.tif a4eqhist.tif 400 400
~/cips/side a4eq.tif a4eqhist.tif a4eqside.tif Side-by-side

~/cips/side a4side.tif a4eqside.tif a4final.tif Top-to-bottom

rm a4eq.tif
rm a4eqhist.tif
rm a4eqside.tif
rm a4hist.tif
rm a4side.tif

