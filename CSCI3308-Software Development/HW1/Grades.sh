#! /bin/bash
#Adam Ten Hoeve

if [ $# -eq 0 ]
then
	echo "Usage: Grades.sh filename"
	exit
fi

filename=$1
sort -t' ' -k3,3 -k2,2 -k1,1 $filename -o $filename
while read -r col1 col2 col3 col4 col5 col6
do
	echo $(((col4+col5+col6)/3)) [$col1] $col3, $col2
done < $filename
