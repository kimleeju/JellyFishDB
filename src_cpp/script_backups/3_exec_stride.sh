
ops="put get range_query"
threads="1 2 4"
dir="../trc/dbbench_trc/1000trc"
conf="ConcurrentSkipList StrideSkipList"

echo "" > stride.rslt
for cf in $conf; do
	for th in $threads; do
		for op in $ops; do 
			#echo "./Run $cf $th $dir/th"$th"_"$op"_load.trc $dir/th"$th"_"$op"_run.trc"
			result=`./Run $cf $th $dir/th"$th"_"$op"_load.trc $dir/th"$th"_"$op"_run.trc`
			echo $cf $th $op $result >> stride.rslt
		done
	done
done

cat stride.rslt

for op in $ops; do
	grep $op stride.rslt
done
#for op in $ops; do
#	crslt=`grep "$op" stride.rslt | grep "Concurrent"`
#	srslt=`grep "$op" stride.rslt | grep "Stride"`
#	echo $crslt $srslt	
#done
