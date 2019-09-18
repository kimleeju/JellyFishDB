
TRC_DIR=../../trc/micro_trc
conf="BlockedSkipList ConcurrentSkipList JellyFishSkipList SimpleSkipList"
threads="1 2 4"
op="unique duplicate"
mode="write read"

for th in $threads; do
	for cf in $conf; do
		rfname=${cf}_${op[1]}_key/${cf}_${op[1]}_key_${mode[1]}_$th.rslt
		trc=$TRC_DIR/micro_${op[1]}_key_${mode[1]}/micro_${op[1]}_key_${mode[1]}_th$th.log	
		echo ".././Run $cf $th $trc  ..."
		.././Run $cf $th $trc  
		#cat $rfname
		grep -rn "IOPS" $rfname
	done
done

