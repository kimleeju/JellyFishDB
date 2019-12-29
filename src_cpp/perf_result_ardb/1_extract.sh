#srcfile="perf_1000000_uni.rslt"

skiplists="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JellyFishSkipList"
#skiplists="ConcurrentSkipList StrideSkipList"
threads="1 2 4 8 16 "
#ops="put get range_query"
#ops="put get"

workload="a b c d e f"

for wo in $workload; do
	srcfile="ardb_workload_"$wo".rslt"
	echo "$srcfile"
	datafile="ardb_workload_"$wo".dat"

	echo "thread $skiplists" > $datafile
	for th in $threads; do
		line="$th "
		for sk in $skiplists; do
			line=$line`grep $sk $srcfile | grep "run" | awk -v sl="$sk" '$1==sl && $7=='"${th}"' {print $NF, s}'`
		done
		echo $line >> $datafile
	done
	cat $datafile
done
