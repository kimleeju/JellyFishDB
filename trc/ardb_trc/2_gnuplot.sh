workloads="a b c d e f"

#workloads="a"

operation="load run"
operation="all"
#operation="load"

for wl in $workloads; do
	for op in $operation; do
		datafile=ardb_8_"$wl"_$op.dat
		outfile=ardb_8_"$wl"_$op.eps
		graphtitle=ardb_8_"$wl"_workloads

		opt_list="`echo '-'``echo 'e'` `echo datafile=\'$datafile\'`"
		opt_list="$opt_list `echo '-'``echo 'e'` `echo outfile=\'$outfile\'`"
		opt_list="$opt_list `echo '-'``echo 'e'` `echo gtitle=\'$graphtitle\'`"
		echo "gnuplot $opt_list skewness.cfg"
		gnuplot $opt_list skewness.cfg
		open $outfile
	done
done
