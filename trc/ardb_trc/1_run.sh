workloads="a b c d e f"

for wl in $workloads; do
	echo "... workloads $wl ..."
	cat ycsb_ardb_8_"$wl"_run.trc | grep "PUT" | awk '{print $2}' | sort | uniq -c | awk '{print $1}' | sort -rn > ardb_8_"$wl"_run.dat
done
