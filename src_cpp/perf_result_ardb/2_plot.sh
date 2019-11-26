
workloads="a b c d e f"

for wk in $workloads; do
	datafile="ardb_workload_"$wk".dat"
	outfile="ardb_workload_"$wk".eps"
	graphtitle="ardb_workload_"$wk""
	
    	opt_list="`echo '-'``echo 'e'` `echo datafile=\'$datafile\'`"
    	opt_list="$opt_list `echo '-'``echo 'e'` `echo outfile=\'$outfile\'`"
	opt_list="$opt_list `echo '-'``echo 'e'` `echo gtitle=\'$graphtitle\'`"

	echo "gnuplot $opt_list perf.cfg" 
	gnuplot $opt_list perf.cfg
	open $outfile
done
