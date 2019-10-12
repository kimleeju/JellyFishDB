datafile="ss.dat"
srcfile="perf_1000000_uni.rslt"

skiplists="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList"
threads="1 2 4"

echo "thread $skiplists"
for th in $threads; do
	line="$th "
	for sk in $skiplists; do
		line=$line`grep $sk $srcfile | grep "run" | awk -v sl="$sk" '$1==sl && $7=='"${th}"' {print $NF, s}'`
		#grep $sk $srcfile | grep "run" | awk '$7=='"${th}"' && $1=='"${sk}"' {print $1}'
	done
	echo $line
done
