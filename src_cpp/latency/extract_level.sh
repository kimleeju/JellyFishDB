
if [[ $# < 1 ]]; then
	echo "Usage: ./sh jdk_all.level"
	exit
fi

sort $1 | uniq -c | sort -rn | grep -v [a-z] | awk '{print $2, $1}' > tmp

cat tmp
awk '{s+=$1*$2} END {printf("Total pointer updates = %d\n", s)}' tmp
