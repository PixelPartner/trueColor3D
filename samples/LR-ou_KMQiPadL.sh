#!/usr/bin/sh
ffmpeg -y -i "/Users/thomas/Documents/trueColor3D/samples/LR-ou.mp4" -aspect 4:3 -vf "[in]select=1,crop='iw:ih/2:0:0',scale=768:432[A];movie=/Users/thomas/Documents/trueColor3D/samples/LR-ou.mp4,select=1,crop='iw:ih/2:0:ih/2',scale=768:432,pad=768:1024:0:40:black[B];[B][A]overlay=0:552,transpose=1" -f mp4 "/Users/thomas/Documents/trueColor3D/samples/LR-ou_KMQiPadL.tmp"
qt-faststart "/Users/thomas/Documents/trueColor3D/samples/LR-ou_KMQiPadL.tmp" "/Users/thomas/Documents/trueColor3D/samples/LR-ou_KMQiPadL.mp4"
rm "/Users/thomas/Documents/trueColor3D/samples/LR-ou_KMQiPadL.tmp"
