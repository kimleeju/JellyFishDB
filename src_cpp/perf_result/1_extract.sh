#srcfile="perf_1000000_uni.rslt"

skiplists="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList"
threads="1 2 4"
ops="put get range_query"


num="1000000"
workloads="uni zipf_1.4"

for op in $ops; do
	for wk in $workloads; do
		srcfile="perf_"$op"_"$wk".rslt"
		echo "$srcfile"
		datafile="perf_"$op"_"$wk".dat"
	
		echo "thread $skiplists" > $datafile
		for th in $threads; do
			line="$th "
			for sk in $skiplists; do
				line=$line`grep $sk $srcfile | grep "run" | awk -v sl="$sk" '$1==sl && $7=='"${th}"' {print $NF, s}'`
				#grep $sk $srcfile | grep "run" | awk '$7=='"${th}"' && $1=='"${sk}"' {print $1}'
			done
			echo $line >> $datafile
		done
		cat $datafile
	done
done
