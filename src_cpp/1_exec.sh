THREADS=1
#SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList StrideSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
#SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
#SKIPLISTS="JellyFishSkipList"
#SKIPLISTS="SimpleSkipList"

SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList StrideSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
SKIPLISTS="ConcurrentSkipList JDKSkipList JellyFishSkipList"
SKIPLISTS="ConcurrentSkipList"
SKIPLISTS="JDKSkipList"
#SKIPLISTS="ConcurrentSkipList"

num="100000"

TRC_DIR="../trc/micro_trc/backup"$num"trc"
#TRC_DIR="../trc/micro_trc/backup100000trc/result"
OP="put get range_query"
OP="put"
CONF="uni zipf_1.4"
CONF="uni"

#CONF="zipf_1.4"

RSLT_DIR="./perf_result"
RSLT_DIR="./perf_result_tmp"

th=$THREADS

## uniform 
	#Usage: ./Run Options thread_count run_trc load_trc 
#for i in {1..1}; do
suffix=`date +%y%m%d_%H%M_%s`
#echo $rfname 
#
#if [[ -f $rfname ]]; then
#	rm $rfname
#fi
#touch $rfname
#echo "" > $rfname
#

for op in $OP; do
	#rfname="$RSLT_DIR/perf_"$suffix"_"$op".rslt"
	echo "$op ....."
	for cf in $CONF; do
		rfname="$RSLT_DIR/perf_"$op"_"$cf"_"$num".rslt"
		rfname="tmp.lat"
		if [[ -f $rfname ]];then
			mv $rfname $rfname.bak
		fi
		touch $rfname
		echo "$rfname ...."

		th=1
		while [[ $th -le $THREADS ]]; do
			for sk in $SKIPLISTS; do
				load_trc=$TRC_DIR/"$cf"_"$op"_load.trc
				run_trc=$TRC_DIR/"$cf"_"$op"_run.trc
				echo "./Run $sk $th $load_trc $run_trc"
				#./Run $sk $th $load_trc $run_trc
				#./Run $sk $th $load_trc $run_trc 
				./Run $sk $th $load_trc $run_trc >> $rfname
			done
			th=$((th+th))
		done
		cat $rfname
		echo "============="
		cat $rfname | grep "run" | awk '{print $1, $NF}'
	done
done

