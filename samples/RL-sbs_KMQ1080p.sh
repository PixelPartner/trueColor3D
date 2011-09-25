#!/usr/bin/sh
ffmpeg -y -i "/Users/thomas/Documents/trueColor3D/samples/RL-sbs.mp4" -b 8500K -aspect 3:4 -vf "[in]select=1,crop='iw/2:ih:0:0',scale=810:455[A];movie=/Users/thomas/Documents/trueColor3D/samples/RL-sbs.mp4,select=1,crop='iw/2:ih:iw/2:0',scale=810:455,pad=810:1080:0:42:black[B];[B][A]overlay=0:582" -f mp4 "/Users/thomas/Documents/trueColor3D/samples/RL-sbs_KMQ1080p.tmp"
qt-faststart "/Users/thomas/Documents/trueColor3D/samples/RL-sbs_KMQ1080p.tmp" "/Users/thomas/Documents/trueColor3D/samples/RL-sbs_KMQ1080p.mp4"
rm "/Users/thomas/Documents/trueColor3D/samples/RL-sbs_KMQ1080p.tmp"
