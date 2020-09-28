#!/bin/bash
#Adam Ten Hoeve

filename=$1
if [ $filename -eq "" ]
then
	echo "Usage: RegexAnswers.sh filename"
	exit
fi

first= grep '[0-9]$' $filename | wc -l
echo $first

second= grep '^[^aeiouAEIOU]' $filename | wc -l
echo $second

third= grep '^[A-Za-z]\{12\}$' $filename | wc -l
echo $third

fourth= grep '[0-9]\{3\}-[0-9]\{3\}-[0-9]\{4\}$' $filename | wc -l
echo $fourth

fifth= grep '303-[0-9]\{3\}-[0-9]\{4\}$' $filename | wc -l
echo $fifth

sixth= grep '^[aeiouAEIOU].*[0-9]$' $filename | wc -l
echo $sixth

seventh= grep '.*@geocities.com$' $filename | wc -l
echo $seventh

eighth= egrep '[A-Za-z0-9._-]+@[a-zA-Z0-9\-]+[^\.][a-zA-Z]{2,}$' $filename | wc -l
echo $eighth
