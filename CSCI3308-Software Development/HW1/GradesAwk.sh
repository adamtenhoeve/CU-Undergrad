#!/bin/bash
# Adam Ten Hoeve

if [ $# -eq 0 ]
then
	echo "Usage: GradesAwk.sh filename"
	exit
fi

filename=$1
sort -t' ' -k3,3 -k2,2 -k1,1 $filename | awk '{print int(($4+$5+$6)/3), "[" $1 "] " $3 ", " $2}'

