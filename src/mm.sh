

n=1
ops=320000
op=$1

while [[ $n -le 32 ]]; do
#	echo "./cskip $n $ops 1"
	result=`./cskip $n $ops $op`
	echo $n $result
	n=$((n+n))
done
