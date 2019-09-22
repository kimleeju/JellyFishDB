THREADS=16
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList SimpleSkipList JellyFishSkipList"

TRC_DIR="../trc/micro_trc/backup10000trc/result"
OP="put get range_query"
CONF="uni zipf_1.2 zipf_1.4 zipf_1.6 zipf_1.8 zipf_2.0"


th=$THREADS

## uniform 
	#Usage: ./Run Options thread_count run_trc load_trc 
for i in {1..10}; do
	for cf in $CONF; do
		for op in $OP; do
			rfname=result/micro_trc/ex$i/"$cf"_"$op".rslt 
#			rfname=result/micro_trc/10000trc/"$cf"_"$op".rslt 
			echo "" > $rfname
			for sk in $SKIPLISTS; do
			
				load_trc=$TRC_DIR/"$cf"_"$op"_load.trc
				run_trc=$TRC_DIR/"$cf"_"$op"_run.trc
				echo $load_trc
				echo $run_trc
				result=`./Run $sk $th $load_trc $run_trc` 
				echo $sk $result >> $rfname
			done
		done
	done
done
