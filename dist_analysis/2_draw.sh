DIR=../trc/orig_trc
conf="load_a run_a run_b run_c run_d run_e"

#for fname in $DIR/*.log; do
for cf in $conf; do
	fname=ycsb_"$cf"_th1

	dfname=$fname.nkey
	ofname=$fname.eps

#	ytitle="Key"
#	xtitle="Logical Time (OP)"
	#gtitle="$store-$sz-"$itv"-$metric"

	flist="`echo '-'``echo e` `echo "dfname='$dfname'"`"
#	flist="$flist `echo '-'``echo e` `echo "ytitle='$ytitle'"`"
#	flist="$flist `echo '-'``echo e` `echo "xtitle='$xtitle'"`"

	flist="$flist `echo '-'``echo e` `echo "ofname='$ofname'"`"
	
	gnuplot $flist ./dist.cfg
	open "$ofname"

done

