#!/usr/bin/sh
ffmpeg -y -i "/Users/thomas/Documents/trueColor3D/samples/LR-sbs.mp4" -b 8500K -aspect 2.66667 -vf "[in]select=1,crop='iw/2:ih:0:0',scale=874:491,pad=1920:720:10:114:black[A];movie=/Users/thomas/Documents/trueColor3D/samples/LR-sbs.mp4,select=1,crop='iw/2:ih:iw/2:0',scale=874:491[B];[A][B]overlay=936:114" -f mp4 "/Users/thomas/Documents/trueColor3D/samples/LR-sbs_my3D720p.tmp"
qt-faststart "/Users/thomas/Documents/trueColor3D/samples/LR-sbs_my3D720p.tmp" "/Users/thomas/Documents/trueColor3D/samples/LR-sbs_my3D720p.mp4"
rm "/Users/thomas/Documents/trueColor3D/samples/LR-sbs_my3D720p.tmp"
