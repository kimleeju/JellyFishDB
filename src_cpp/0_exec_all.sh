THREADS=8
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList SimpleSkipList"

TRC_DIR="../trc/micro_trc"
OP="put get range_query"
CONF="uni zipf_1.2 zipf_1.4 zipf_1.6 zipf_1.8 zipf_2.0"


th=$THREADS

## uniform 
for sk in $SKIPLISTS; do
	#Usage: ./Run Options thread_count run_trc load_trc 
	for cf in $CONF; do
		for op in $OP; do
			load_trc=$TRC_DIR/"$cf"_"$op"_load.trc
			run_trc=$TRC_DIR/"$cf"_"$op"_run.trc
			echo $load_trc
			echo $run_trc
			echo "./Run $sk $th $load_trc $run_trc"
		done
	done
done
