make clean
make -j4
#CLASSPATH=$CLASSPATH:~/JDKSkipList/
#export $CLASSPATH

THREADS=16
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList StrideSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList"
SKIPLISTS="BlockedSpinSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList"
RSLT_DIR="./perf_result_191103"

OP="put"
CONF="uni"

if [[ ! -f $RSLT_DIR ]]; then
	mkdir $RSLT_DIR
fi

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


for nu in {1..1}; do
	RSLT_DIR="./perf_result_range_query/$nu"
	if [[ ! -f $RSLT_DIR ]]; then
		mkdir $RSLT_DIR
	fi

	for op in $OP; do
		#rfname="$RSLT_DIR/perf_"$suffix"_"$op".rslt"
		echo "$op ....."
		for cf in $CONF; do
			rfname="$RSLT_DIR/perf_"$op"_"$cf"_"$num".rslt"
			if [[ -f $rfname ]];then
				mv $rfname $rfname.bak
			fi
			touch $rfname
			echo "$rfname ...."
	
			th=1
			while [[ $th -le $THREADS ]]; do
				for sk in $SKIPLISTS; do
					iter=5
					while [[ $iter -le 5 ]]; do
						#rfname="latency/$sk.lat"
						load_trc=$TRC_DIR/"$cf"_"$op"_load.trc
						run_trc=$TRC_DIR/"$cf"_"$op"_run.trc
						echo "./Run $sk $th $load_trc $run_trc"
						#./Run $sk $th $load_trc $run_trc
						#./Run $sk $th $load_trc $run_trc
#						echo 3 > /proc/sys/vm/drop_caches
#						sleep .5
						./Run $sk $th $load_trc $run_trc >> $rfname
#						sleep .5
						#./Run $sk $th $load_trc $run_trc > con.level
						iter=$((iter+1))
					done
				done
				th=$((th+th))
			done
			cat $rfname | grep "run" | awk '{print $1, $NF}'
			cat $rfname | grep "comparator" 
		done
	done
done
