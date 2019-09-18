THREADS=8
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList SimpleSkipList"

TRC_DIR="../trc/micro_trc"
TRACES=$TRC_DIR/*

## uniform 
for sk in $SKIPLISTS; do
	#Usage: ./Run Options thread_count run_trc load_trc 

	for rtrc in $S
	./Run $sk $THREADS 

done
