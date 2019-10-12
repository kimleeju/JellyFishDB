
ops="1000000"
workloads="uni zipf_1.4"

for wk in $workloads; do
	datafile="perf_"$ops"_"$wk".dat"
	outfile="perf_"$ops"_"$wk".eps"
	
    opt_list="`echo '-'``echo 'e'` `echo datafile=\'$datafile\'`"
    opt_list="$opt_list `echo '-'``echo 'e'` `echo outfile=\'$outfile\'`"

    echo "gnuplot $opt_list perf.cfg" 
    gnuplot $opt_list perf.cfg
    open $outfile
done
