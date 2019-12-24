workloads="a b c d e f"

for wl in $workloads; do
	echo "... workloads $wl ..."
	cat ycsb_ardb_8_"$wl".trc |grep "PUT" | awk '{print $2}' | sort | uniq -c | awk '{print $1}' | sort -rn | head -n 10 > ardb_8_"$wl"_all.dat
done
