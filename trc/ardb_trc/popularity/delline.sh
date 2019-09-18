### delete line (needless)
workloads="b c d e f"
threads="1 2 4 8"

sed -i '1,310d' ardb_1_a.log
sed -i '180000, 180014d' ardb_1_a.log

for wl in $workloads; do
	for th in $threads; do
		sed -i '1,310d' ardb_"$th"_"$wl".log
		sed -i '$d' ardb_"$th"_"$wl".log
	done
done
