make clean
make -j4
#CLASSPATH=$CLASSPATH:~/JDKSkipList/
#export $CLASSPATH

THREADS=8
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList StrideSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JellyFishSkipList"
#SKIPLISTS="BlockedSpinSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList"

#SKIPLISTS="BlockedSpinSkipList ConcurrentSkipList JellyFishSkipList"
#SKIPLISTS="BlockedCVSkipList"
#SKIPLISTS="BlockedCVSkipList"

#SKIPLISTS="BlockedCVSkipList"
#SKIPLISTS="JellyFishSkipList"
SKIPLISTS="ConcurrentSkipList StrideSkipList"

workload="a b c d e f"
#SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
#num="100"

TRC_DIR="../trc/ardb_trc"
#TRC_DIR="backup"$num"trc"
#TRC_DIR="../trc/micro_trc/backup100000trc/result"
#OP="put get range_query"
#OP="range_query"
#OP="put"
#OP="range_query"
#OP=" put"
#OP="get" 
RSLT_DIR="./perf_result_stride_ardb"
#RSLT_DIR="./perf_result_count"

#OP="put"
#CONF="uni dup_2 zipf_1.2"
#CONF="dup_2"
#CONF="zipf_1.2"
#CONF="uni"
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
#	RSLT_DIR="./perf_result_range_query/$nu"
	if [[ ! -f $RSLT_DIR ]]; then
		mkdir $RSLT_DIR
	fi

	for wo in $workload; do
		#rfname="$RSLT_DIR/perf_"$suffix"_"$op".rslt"
		echo "$op ....."
	#	for cf in $CONF; do
			rfname="$RSLT_DIR/ardb_workload_"$wo".rslt"
			if [[ -f $rfname ]];then
				mv $rfname $rfname.bak
			fi
			touch $rfname
			echo "$rfname ...."
	
			th=8
			while [[ $th -le $THREADS ]]; do
				for sk in $SKIPLISTS; do
					iter=5
					while [[ $iter -le 5 ]]; do
						#rfname="latency/$sk.lat"
						load_trc=$TRC_DIR/ycsb_ardb_8_"$wo"_load.trc
						run_trc=$TRC_DIR/ycsb_ardb_8_"$wo"_run.trc
						echo "./Run $sk $th $load_trc $run_trc"
						#./Run $sk $th $load_trc $run_trc
						#./Run $sk $th $load_trc $run_trc
#						echo 3 > /proc/sys/vm/drop_caches
#						sleep .5
						
#						./Run $sk $th $load_trc $run_trc
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
	#	done
	done
done
