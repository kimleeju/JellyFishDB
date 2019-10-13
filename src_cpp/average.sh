skiplists="ConcurrentSkipList JDKSkipList JellyFishSkipList"

for sk in $skiplists; do
	fname="$sk".lat
	avg=`awk '{s+=$1} END {print s/NR}' $fname`
	echo $sk $avg

done
