
num="1000000"
ops="get"
workloads="uni zipf_1.2"

for op in $ops; do
	for wk in $workloads; do
		datafile="perf_"$op"_"$wk"_"$num".dat"
		outfile="perf_"$op"_"$wk"_"$num".eps"
		graphtitle="$op"-"$wk"-"$num"
	
	opt_list="`echo '-'``echo 'e'` `echo datafile=\'$datafile\'`"
   	opt_list="$opt_list `echo '-'``echo 'e'` `echo outfile=\'$outfile\'`"
   	opt_list="$opt_list `echo '-'``echo 'e'` `echo gtitle=\'$graphtitle\'`"

   	echo "gnuplot $opt_list perf.cfg" 
   	gnuplot $opt_list perf_pointer_update.cfg
   	open $outfile
	done
done
