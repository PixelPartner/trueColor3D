#!/usr/bin/sh
ffmpeg -y -i "/Users/thomas/Documents/trueColor3D/samples/LR-ou.mp4" -b 4500K -aspect 960:244 -vf "[in]select=1,crop='iw:ih/2:0:ih/2',scale=436:244[A];movie=/Users/thomas/Documents/trueColor3D/samples/LR-ou.mp4,select=1,crop='iw:ih/2:0:0',scale=436:244,pad=960:244:8:0:black[B];[B][A]overlay=468:0" -f mp4 "/Users/thomas/Documents/trueColor3D/samples/LR-ou_my3D960.tmp"
qt-faststart "/Users/thomas/Documents/trueColor3D/samples/LR-ou_my3D960.tmp" "/Users/thomas/Documents/trueColor3D/samples/LR-ou_my3D960.mp4"
rm "/Users/thomas/Documents/trueColor3D/samples/LR-ou_my3D960.tmp"
