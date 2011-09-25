#!/usr/bin/sh
ffmpeg -y -i "/Users/thomas/Documents/trueColor3D/samples/RL-ou.mp4" -b 4500K -aspect 2.66667 -vf "[in]select=1,crop='iw/2:ih:iw/2:0',scale=582:327[A];movie=/Users/thomas/Documents/trueColor3D/samples/RL-ou.mp4,select=1,crop='iw/2:ih:0:0',scale=582:327,pad=1280:480:10:76:black[B];[B][A]overlay=624:76" -f mp4 "/Users/thomas/Documents/trueColor3D/samples/RL-ou_my3D480p.tmp"
qt-faststart "/Users/thomas/Documents/trueColor3D/samples/RL-ou_my3D480p.tmp" "/Users/thomas/Documents/trueColor3D/samples/RL-ou_my3D480p.mp4"
rm "/Users/thomas/Documents/trueColor3D/samples/RL-ou_my3D480p.tmp"
