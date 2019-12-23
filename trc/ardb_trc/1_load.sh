workloads="a b c d e f"

for wl in $workloads; do
	echo "... workloads $wl ..."
	grep "PUT" ycsb_ardb_8_"$wl"_load.trc | awk '{print $2}' | sort | uniq -c | awk '{print $1}' | sort -rn > ardb_8_"$wl"_load.dat
done
