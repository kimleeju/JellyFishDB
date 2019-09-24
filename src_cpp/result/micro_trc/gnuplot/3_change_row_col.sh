#!/bin/bash

infile="ex5/put.rslt ex5/get.rslt ex5/range_query.rslt"
#infile=$1
declare -a array

array=(`cat $infile`)
ncol=`head -1 $infile | wc -w`
nrow=`awk '{printf("%d\n", $1)}' $infile | wc -w`

#echo "$ncol"; 6
#echo "$nrow"; 5

#for f in $infile; do
	#array=(`cat $f`)
	for ((i=0;i<$ncol;i++)); do
		for ((j=0;j<$nrow;j++)); do
			echo -n ${array[j*ncol+i]} >> $infile.dat
			echo -n " " >> $infile.dat
		done
		echo ""  >> $infile.dat
	done
#done
