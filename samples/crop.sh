#!/usr/bin/sh
ffmpeg -y -i "/Users/thomas/Documents/trueColor3D/samples/LR-ou.mp4" -aspect 960:244 -vf "[in]select=1,crop='320:180:0:180'[out]" -f mp4 "/Users/thomas/Documents/trueColor3D/samples/crop.mp4"
