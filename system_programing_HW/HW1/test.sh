#!/bin/sh
#by LCH
TESTDATALOG=./testData/;
num=1
while [ $num -le 3 ]; do
	if test $# -lt 1;
	then
		./generator.pl -100000 100000 ${num}0000000 > ku_ps_input.h
		mkdir -p ${TESTDATALOG}\test${num}
		cp ku_ps_input.h ${TESTDATALOG}\test${num}/ku_ps_input.h
		gcc ku_ff.c -o \test${num}
	else
		cp ku_ff.c ku_ff_test_${num}.c;
		echo "#include \"${TESTDATALOG}test${num}/ku_ps_input.h\"" > tempdata.txt
		cat tempdata.txt ku_ff_test_${num}.c > ku_ff_test${num}.c
		gcc ku_ff_test${num}.c -DREDO -o \test${num};
		rm ku_ff_test${num}.c ku_ff_test_${num}.c tempdata.txt;
	fi

	if [ $# = 1 ]
	then
		num1=1
		while [ $num1 -le 5 ]; do
			num2=`expr $num1 "*" $1`
			echo "<fork$num2>"
			./\test${num} 0 1000 $num2;
			num1=`expr $num1 + 1`
		done
	elif [ $# = 2 ]
		then
		if [ $1 = time ]
		then
			num1=1
			while [ $num1 -le 5 ]; do
				num2=`expr $num1 "*" $2`
				echo "<fork$num2>"
				gdate +"%T.%3N";
				./\test${num} 0 1000 $num2;
				gdate +"%T.%3N";
				num1=`expr $num1 + 1`
			done
		else
			num1=1
			while [ $num1 -le 5 ]; do
				num2=`expr $num1 "*" $2`
				echo "<fork$num2>"
				./\test${num} 0 1000 $num2;
				num1=`expr $num1 + 1`
			done
		fi
	else
		./\test${num} 100 1000 10;		
	fi
	rm ./\test${num}
	num=`expr $num + 1`
done
