THREADS=4
#SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList StrideSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
#SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
#SKIPLISTS="JellyFishSkipList"
#SKIPLISTS="SimpleSkipList"

SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList StrideSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList SimpleSkipList JellyFishSkipList"

ops="1000000"
TRC_DIR="../trc/micro_trc/backup"$ops"trc"
#TRC_DIR="../trc/micro_trc/backup100000trc/result"
OP="put get range_query"
OP="put"
CONF="uni zipf_1.4"
CONF="zipf_1.4"

RSLT_DIR="./perf_result"


th=$THREADS

## uniform 
	#Usage: ./Run Options thread_count run_trc load_trc 
#for i in {1..1}; do
suffix=`date +%y%m%d_%H%M_%s`
suffix="$ops"_"$CONF"
rfname="$RSLT_DIR/perf_"$suffix".rslt"
echo $rfname 
if [[ -f $rfname ]]; then
	rm $rfname
fi
touch $rfname

th=1
while [[ $th -le $THREADS ]]; do
	for cf in $CONF; do
		for op in $OP; do
		
			for sk in $SKIPLISTS; do
			
				load_trc=$TRC_DIR/"$cf"_"$op"_load.trc
				run_trc=$TRC_DIR/"$cf"_"$op"_run.trc
				echo "./Run $sk $th $load_trc $run_trc"
				./Run $sk $th $load_trc $run_trc >> $rfname
			done
		done
	done
	th=$((th+th))
done

cat $rfname
