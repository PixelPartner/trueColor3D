#!/usr/bin/sh
ffmpeg -y -i "/Users/thomas/Documents/trueColor3D/samples/RL-sbs.mp4" -b 8500K -aspect 1280:1040 -vf "[in]select=1,crop='iw/2:ih:0:0',scale=1040:588[A];movie=/Users/thomas/Documents/trueColor3D/samples/RL-sbs.mp4,select=1,crop='iw/2:ih:iw/2:0',scale=1040:588,pad=1040:1280:0:0:black[B];[B][A]overlay=0:692,transpose=1" -f mp4 "/Users/thomas/Documents/trueColor3D/samples/RL-sbs_KMQ1280.tmp"
qt-faststart "/Users/thomas/Documents/trueColor3D/samples/RL-sbs_KMQ1280.tmp" "/Users/thomas/Documents/trueColor3D/samples/RL-sbs_KMQ1280.mp4"
rm "/Users/thomas/Documents/trueColor3D/samples/RL-sbs_KMQ1280.tmp"
