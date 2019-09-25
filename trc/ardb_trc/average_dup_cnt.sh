for f in *.trc; do
	cnt=`awk '{print $2}' $f | sort | uniq -c | sort -nr | awk '{s+=$1} END {print s/NR}'`
	echo $f $cnt

done
