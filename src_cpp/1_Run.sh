TRC_DIR=../trc/micro_trc
conf="BlockedSkipList ConcurrentSkipList JellyFishSkipList"
threads="1 2 4"

for th in $threads; do
	for cf in $conf; do
		rfname=$cf.rslt
		trc=$TRC_DIR/test.log
		echo "./Run $cf $th $trc  ..."
		./Run $cf $th $trc > $rfname 
		cat $rfname
		#grep -rn "IOPS" $rfname
	done
done
