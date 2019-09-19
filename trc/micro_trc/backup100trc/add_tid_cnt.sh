### add tid, cnt of range_query
skewness="uni zipf_1.2 zipf_1.4 zipf_1.6 zipf_1.8 zipf_2.0"
operations="get put range_query"
wl_type="load run"

for sn in $skewness; do
	for op in $operations; do
		for wt in $wl_type; do
			if [ $op == "range_query" ] && [ $wt == "run" ]; then
				awk '{printf("%d %s %s %d\n", NR%16, $1, $2, 10)}' "$sn"_"$op"_"$wt".trc > result/"$sn"_"$op"_"$wt".trc
			else
				awk '{printf("%d %s %s\n", NR%16, $1, $2)}' "$sn"_"$op"_"$wt".trc > result/"$sn"_"$op"_"$wt".trc
			fi
		done
	done
done
