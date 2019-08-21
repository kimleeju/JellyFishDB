#!/bin/bash
threads="1 2 4 8 16"
#threads="1 2 4"
skips="skip cskip vcskip"
num=50000
benchmarks="1 2 3" ##put get scan

cp ~/skip_vchain/src/skip .
cp ~/skip_vchain/src/cskip .
cp ~/skip_vchain/src/vcskip .
for bench in $benchmarks
do
	echo "   "  "skip" "cskip" "vcksip" 
	for thread in $threads
	do
		skip=$(./skip $thread $num $bench)
		sleep 1 
		cskip=$(./cskip $thread $num $bench)
		sleep 1 
		vcskip=$(./vcskip $thread $num $bench)
		sleep 1
		echo "$thread" "$skip" "$cskip" "$vcskip" 
	done
	echo " "
	
done
