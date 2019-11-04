
echo "all ... "
for f in ./*.trc; do
	echo $f `awk '{print $2}' $f | sort | uniq -c | sort -nr | awk '{s+=$1} END {print s/NR}'`
done



echo "PUT ... "
for f in ./*.trc; do
	echo $f `awk '$1=="PUT" {print $2}' $f | sort | uniq -c | sort -nr | awk '{s+=$1} END {print s/NR}'`
done

echo "GET ... "
for f in ./*.trc; do
	echo $f `awk '$1=="GET" {print $2}' $f | sort | uniq -c | sort -nr | awk '{s+=$1} END {print s/NR}'`
done

