THREADS=16
#SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList StrideSkipList JDKSkipList SimpleSkipList"

SKIPLISTS="ConcurrentSkipList JellyFishSkipList"

TRC_DIR="../trc/micro_trc/backup10000trc/result"
OP="put get range_query"
CONF="uni  zipf_1.4"
RSLT_DIR="./perf_result"


th=$THREADS

## uniform 
	#Usage: ./Run Options thread_count run_trc load_trc 
#for i in {1..10}; do
	for cf in $CONF; do
		for op in $OP; do
#			rfname=$RSLT_DIR/"$cf"_"$op"_"$i".rslt 
			#rfname=result/micro_trc/ex"$i"/"$cf"_"$op".rslt 
			
			rfname=result/micro_trc/compare/"$cf"_"$op".rslt 
			echo "" > $rfname
			echo "" > $rfname
			for sk in $SKIPLISTS; do
			
				load_trc=$TRC_DIR/"$cf"_"$op"_load.trc
				run_trc=$TRC_DIR/"$cf"_"$op"_run.trc
				echo "$load_trc"_"$sk"
				echo "$run_trc"_"$sk"
				result=`./Run $sk $th $load_trc $run_trc` 
	#			echo $sk $result
				echo $sk $result >> $rfname
			done
		done
	done
#done
