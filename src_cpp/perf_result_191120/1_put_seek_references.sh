#srcfile="perf_1000000_uni.rslt"

skiplists="ConcurrentSkipList JellyFishSkipList"
#skiplists="BlockedSpinSkipList ConcurrentSkipList JellyFishSkipList"
threads="1 2 4 8 16"
ops="put get range_query"
ops="put"
#ops="get"

num="1000000"
workloads="uni dup_2 zipf_1.2"

for op in $ops; do
	for wk in $workloads; do
		srcfile="perf_"$op"_"$wk"_"$num".rslt"
		datafile="perf_"$op"_"$wk"_"$num".dat"
		echo "threads ConcurrentSkipList JellyFishSkipList" >$datafile
		for th in $threads; do
			echo $th | tr "\n" " " >> $datafile

			grep -A 26 "th = $th " $srcfile | grep "CC PUT_SEEK = "  | awk '{sum += $6}END{print sum}' | tr "\n" " " >> $datafile
			grep -A 26 "th = $th " $srcfile | grep "JF PUT_SEEK = "  | awk '{sum += $6}END{print sum}' | tr "\n" " " >> $datafile
			echo "" >> $datafile	
		done
	done
done
