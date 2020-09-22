#srcfile="perf_1000000_uni.rslt"

skiplists="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JellyFishSkipList"
#skiplists="BlockedSpinSkipList ConcurrentSkipList JellyFishSkipList"
threads="1 2 4 8 16"
ops="put get range_query"
ops="put get"
ops="get"

num="1000000"
workloads="uni zipf_1.2"

for op in $ops; do
	for wk in $workloads; do
		srcfile="perf_"$op"_"$wk"_"$num".rslt"
		echo "$srcfile"
		datafile="perf_"$op"_"$wk"_"$num".dat"
	
		echo "thread $skiplists" > $datafile
		for th in $threads; do
			echo "$th" | tr "\n" " " >>$datafile
			for sk in $skiplists; do
				#line=$line`grep $sk $srcfile | grep "run" | awk -v sl="$sk" '$1==sl && $7=='"${th}"' {print $NF, s}'`
				#grep $sk $srcfile | grep "run" | awk '$7=='"${th}"' && $1=='"${sk}"' {print $1}'
				#line=$line grep $sk $srcfile | grep "comparator" | awk '{print $5}'
				#cat "$srcfile" | awk '{if($5 == "th=$th")}'
				
				if [ "$th" -eq 1 ]; then
					head -n 24 "$srcfile" | grep "comparator" |  grep "$sk" |awk '{print $5}' | tr "\n" " " >> $datafile
				elif [ "$th" -eq 2 ]; then
					head -n 48 "$srcfile" | tail -n 24 | grep "comparator" |  grep "$sk" |awk '{print $5}' | tr "\n" " " >> $datafile 
				elif [ "$th" -eq 4 ]; then
					head -n 72 "$srcfile" | tail -n 24 | grep "comparator" |  grep "$sk" |awk '{print $5}' | tr "\n" " " >> $datafile
				elif [ "$th" -eq 8 ]; then
					head -n 96 "$srcfile" | tail -n 24 | grep "comparator" |  grep "$sk" |awk '{print $5}' | tr "\n" " " >> $datafile
				elif [ "$th" -eq 16 ]; then
					head -n 120 "$srcfile" | tail -n 24 | grep "comparator" |  grep "$sk" |awk '{print $5}' | tr "\n" " " >> $datafile
				fi	
				
				#cat "$srcfile" | grep "comparator" | grep "$sk" |  awk '{print $5}' | tr "\n" " " >> $datafile
				#line+= cat "$srcfile" | grep "comparator" | awk '{print $1 $5}'
				#echo $line >> $datafile
			done
			#echo $line >> $datafile
			echo "" >> $datafile
		done
		cat $datafile
	done
done

