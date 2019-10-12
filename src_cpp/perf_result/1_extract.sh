datafile="ss.dat"
srcfile="perf_1000000_uni.rslt"

skiplists="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList"

for sk in $skiplists; do
	grep $sk $srcfile | awk '{print $1}'
done
