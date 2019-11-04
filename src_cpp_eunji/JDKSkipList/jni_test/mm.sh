javac MyConcurrentSkipListMap.java

ops=100000

while [[ $ops -le 100000 ]]; do
	java MyConcurrentSkipListMap $ops
	ops=$((ops * 10))
done


