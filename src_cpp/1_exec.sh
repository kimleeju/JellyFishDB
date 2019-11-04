make clean
make -j4
#CLASSPATH=$CLASSPATH:~/JDKSkipList/
#export $CLASSPATH

THREADS=16
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList StrideSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList"
SKIPLISTS="BlockedSpinSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList"
<<<<<<< HEAD

SKIPLISTS="BlockedSpinSkipList ConcurrentSkipList JellyFishSkipList"
=======
<<<<<<< HEAD
SKIPLISTS="BlockedSpinSkipList ConcurrentSkipList JellyFishSkipList"
##SKIPLISTS="BlockedCVSkipList"
=======
>>>>>>> 4be1f20c017776add17ab668111a44c4a363be8e
#SKIPLISTS="BlockedCVSkipList"
>>>>>>> f7d71e77ff4472257e1e8076d33a668b27bb3e34

#SKIPLISTS="BlockedCVSkipList"
#SKIPLISTS="JellyFishSkipList"
#SKIPLISTS="ConcurrentSkipList JellyFishSkipList"
num="10000"

#SKIPLISTS="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
#num="100"

TRC_DIR="../trc/micro_trc/backup"$num"trc"
#TRC_DIR="backup"$num"trc"
#TRC_DIR="../trc/micro_trc/backup100000trc/result"
OP="put get range_query"
OP="range_query"
<<<<<<< HEAD
OP="put get"
OP="range_query"
#OP="get"

=======
OP="put"
>>>>>>> 4be1f20c017776add17ab668111a44c4a363be8e
CONF="uni zipf_1.2"
#OP="put"
#OP="range_query"
#CONF="uni zipf_1.4"
#CONF="uni"
#CONF="zipf_1.2"
#CONF="uni"
#CONF="zipf_1.4"
#CONF="zipf_1.4"

RSLT_DIR="./perf_result"
RSLT_DIR="./perf_result_tmp"
<<<<<<< HEAD
RSLT_DIR="./perf_result_191104_eun"
=======
RSLT_DIR="./perf_result_191103"
>>>>>>> f7d71e77ff4472257e1e8076d33a668b27bb3e34

RSLT_DIR="./perf_result_range_query"
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
