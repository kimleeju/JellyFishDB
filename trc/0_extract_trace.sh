#!/bin/bash 

list=`ls | grep "log"`
for file in $list
do	
	cat $file | awk -F "user" '{print $1 $2 $3 }' >> "all_trace_ycsb.trc"
done
