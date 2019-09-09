
TRC_DIR=../../trc/micro_trc
conf="BlockedSkipList ConcurrentSkipList JellyFishSkipList SimpleSkipList"
threads="1 2 4"
op="unique duplicate"
mode="write read"

for i in 2; do
	for th in $threads; do
		for cf in $conf; do
			rfname=${cf}_${op[$i]}_key/${cf}_${op[$i]}_key_${mode[0]}_$th.rslt
			trc=$TRC_DIR/micro_${op[$i]}_key_${mode[0]}/micro_${op[$i]}_key_${mode[0]}_th$th.log
			echo ".././Run $cf $th $trc  ..."
			.././Run $cf $th $trc > $rfname 
			#cat $rfname
			grep -rn "IOPS" $rfname
		done
	done
done
