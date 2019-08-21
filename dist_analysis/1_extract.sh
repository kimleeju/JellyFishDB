DIR=../trc/orig_trc
conf="load_a run_a run_b run_c run_d run_e"
conf="run_e"

#for fname in $DIR/*.log; do
for cf in $conf; do
	fname=ycsb_"$cf"_th16
	echo $fname
	awk -F"user" '{print $2}' $DIR/$fname.log | awk '{print $1}' | awk 'NF' | sort -n > $fname.nkey
	#awk -F"user" '{print $2}' $DIR/$fname.log | awk 'NF' | sort | uniq -c | sort -nr > $fname.nkey

done

