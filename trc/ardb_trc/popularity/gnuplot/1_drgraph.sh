workloads="a b c d e f"
threads="1 2 4 8"

for wl in $workloads; do
	for th in $threads; do
		f=../ardb_"$th"_"$wl".rslt
		of="ardb_"$th"_"$wl".eps"
		t=Thread"$th"-"$wl"
		flist="`echo '-'``echo e` `echo "datafile='$f'"`"
		flist="$flist `echo '-'``echo e` `echo "outname='$of'"`"
		flist="$flist `echo '-'``echo e` `echo "title='$t'"`"

	#	./draw_gnuplot.sh ardb $flist
		echo "gnuplot $flist ./ardb.cfg"
		gnuplot $flist ./ardb.cfg
		xdg-open $of
#./draw_gnuplot.sh ardb -e "datafile=../ardb_"$th"_"$wl".rslt"
	done
done
