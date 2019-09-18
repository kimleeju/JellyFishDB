### extract duplicate rank
workloads="a b c d e f"
threads="1 2 4 8"

for wl  in $workloads; do
	for th in $threads; do
		awk '{print $2}' ardb_"$th"_"$wl".log | sort | uniq -c | sort -nr | awk '{print $2, $1}' > ardb_"$th"_"$wl".rrslt
	done
done

for wl in $workloads; do
	for th in $threads; do
		awk '{for(i=1;i<NF;i++) print NR, $2}' ardb_"$th"_"$wl".rrslt > ardb_"$th"_"$wl".rslt
	done 
done

