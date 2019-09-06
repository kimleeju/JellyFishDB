

threads="1 2 4"
workload="unique duplicate"
workload="unique"

for th in $threads; do
	for wk in $workload; do
		echo "threads = $th"
		#grep -rn "IOPS" *key_"$th".rslt
		grep -rn "IOPS" *"$wk"_key_"$th".rslt
	done
done
