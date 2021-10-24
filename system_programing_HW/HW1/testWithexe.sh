#!/bin/sh
num=1;
while [ $num -lt 11 ]; do
	num1=`expr $num "*" $2`
	echo "<fork$num1>"
	StartTime=$(date +"%s%3N")
	./test$1 0 1000 $num1
	EndTime=$(date +"%s%3N")
	echo "\ntime: $(($EndTime - $StartTime)) (ms)"
	num=`expr $num + 1`
done
