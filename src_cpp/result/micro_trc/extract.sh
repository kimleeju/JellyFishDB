
workloads="put get range_query" 

for w in $workloads; do
	for f in *$w.rslt; do
		echo $f
		cat $f
	done
done
