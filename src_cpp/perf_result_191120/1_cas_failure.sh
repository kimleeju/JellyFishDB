#srcfile="perf_1000000_uni.rslt"

skiplists="ConcurrentSkipList JellyFishSkipList"
#skiplists="BlockedSpinSkipList ConcurrentSkipList JellyFishSkipList"
threads="1 2 4 8 16"
ops="put get range_query"
ops="put get"
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
				grep -A 2 "th = $th " $srcfile | grep "CC CAS_FAIL = "  | awk '{print $5}' | tr "\n" " ">>$datafile 
				grep -A 2 "th = $th " $srcfile | grep "JF CAS_FAIL = "  | awk '{print $5}' >>$datafile 
				
		done
	done
done

