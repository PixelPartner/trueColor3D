#!/usr/bin/sh
ffmpeg -y -i "/Users/thomas/Documents/trueColor3D/samples/LR-ou.mp4" -aspect 3:4 -vf "[in]select=1,crop='iw:ih/2:0:0',scale=768:432[A];movie=/Users/thomas/Documents/trueColor3D/samples/LR-ou.mp4,select=1,crop='iw:ih/2:0:ih/2',scale=768:432,pad=768:1024:0:40:black[B];[B][A]overlay=0:552" -f mp4 "/Users/thomas/Documents/trueColor3D/samples/LR-ou_KMQiPadP.tmp"
qt-faststart "/Users/thomas/Documents/trueColor3D/samples/LR-ou_KMQiPadP.tmp" "/Users/thomas/Documents/trueColor3D/samples/LR-ou_KMQiPadP.mp4"
rm "/Users/thomas/Documents/trueColor3D/samples/LR-ou_KMQiPadP.tmp"
