#Converts maps from PNG (as from OSM or TileMill) into
#vertically-flipped RGB565 bitmaps for use with the
#runner's GPS.
#
#First argument is the source directory, second is the
#output directory

#!/bin/sh

IN=$1
OUT=$2

#ffmpeg -v warning -i $1 -pix_fmt rgb565 `echo $1|sed 's/png$/bmp/'`

for Z in `ls $IN`; do
	for X in `ls $IN/$Z/`; do
		for FILE in `ls $IN/$Z/$X/`; do
			mkdir -p $OUT/$Z/$X/
			INPUT=$IN/$Z/$X/$FILE
			OUTPUT=$OUT/$Z/$X/$FILE
			OUTPUT=$(echo $OUTPUT | sed 's/png$/bmp/')
			#echo $OUT/$Z/$X/$FILE
			ffmpeg -v warning -i $INPUT -vf vflip -pix_fmt rgb565 $OUTPUT
		done
	done
done
