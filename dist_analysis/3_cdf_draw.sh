ratio=8
requests="100 1000 10000 100000"
requests=10000
csz=100
trans=5
conf="ori inf thcond exp"
modes="WAL TRUNCATE PERSIST DELETE"
#conf="thcond"

#### result 
for md in $modes; do
	for rt in $ratio; do
		for req in $requests; do
			df1=result_"ori"_"$rt"_"$req"_"$md".lat
			df2=result_"exp"_"$rt"_"$req"_"$md".lat
			of="cdf_"$rt"_"$req"_"$md".eps"
			flist="`echo '-'``echo e` `echo "datafile1='$df1'"`"
			flist="$flist `echo '-'``echo e` `echo "datafile2='$df2'"`"
			flist="$flist `echo '-'``echo e` `echo "ofname='$of'"`"
			
			echo "gnuplot $flist ./cdf.cfg"
			gnuplot $flist ./cdf.cfg
			open $of
#
#			if [[ -e $rfname ]]; then
#					echo $rfname
#				else
#					echo $rfname " does not exist."
#				fi
			#	echo $cf $req `grep Running $rfname | awk '{print $4}'`
		done
		echo ""
	done
done



##
# rados-4k-1th filestore buddy-hash budyd-log

if [[ $# -lt 3 ]]; then
	echo "./.sh latency | bandwidth filestore 16"
	exit

fi


workloads="fs bh bl"
workloads="fs_60s bl_60s"
threads="1 16"
interval="5 10 20 40 80"
interval="5"
size="4096 1048576"
metric=$1
th=$3
store=$2

if [[ $metric == "bandwidth" ]]; then
	cnum=6
fi

if [[ $metric == "latency" ]]; then
	cnum=7
fi

for sz in $size; do
	for itv in $interval; do 
		for w in $workloads; do
			infile="$store"_rados_th16_"$sz"_"$sz".perf
			#infile=time_rados_bluestore_"$sz"_"$sz"_"$itv".perf
			#datafile=rados_"$sz"_"$th"_"$w".$metric
			datafile=$infile.$metric

			grep -v '[a-zA-Z]' $infile | awk '{print $'"${cnum}"'}' | grep [0-9] > $datafile
			echo "$datafile is created"
		done
	done
done

##### plot 
#for sz in $size; do
#	prefix="time_rados_w_"
#	ytitle=$metric
#	xtitle="time(s)"
#	gtitle=$prefix.$metric
#
#	flist="`echo '-'``echo e` `echo "gtitle='$gtitle'"`"
#
#	for itv in $interval; do 
#		workload="$sz"_"$sz"_"$itv"
#		#output="rados_"$sz"_"$th"_time_$metric".eps
#
#		flist="$flist `echo '-'``echo e` `echo "ytitle='$ytitle'"`"
#		flist="$flist `echo '-'``echo e` `echo "xtitle='$xtitle'"`"
#
#		flist="$flist `echo '-'``echo e` `echo interval$itv=\'"$prefix""$workload".perf.$metric\'`"
#		#flist="$flist `echo '-'``echo e` `echo buddystorel=\'"$prefix"_bl_60s.perf.time_$metric\'`"
#
#	done
#
#	output=$prefix"all".$metric.eps
#	flist="$flist `echo '-'``echo e` `echo "oname='$output'"`"
#		
#	echo "gnuplot $flist 6_gnuplot_rados_latency.cfg"
#	gnuplot $flist 4_gnuplot_timeline_perf_plot.cfg
#	open "$output"
#
#done
#
#
#
#### plot 

for sz in $size; do
	for itv in $interval; do 
		#prefix="time_rados_"$store"_"$th"_"$sz"_"$sz"_"$itv""
		prefix="$store"_rados_th16_"$sz"_"$sz"
		#prefix="time_rados_bluestore_"$sz"_"$sz"_"$itv""
		output=$prefix.$metric.eps
		#output="rados_"$sz"_"$th"_time_$metric".eps

		ytitle=""$metric"(MB/s)"
		xtitle="time(s)"
		gtitle="$store-$sz-"$itv"-$metric"

		flist="`echo '-'``echo e` `echo "gtitle='$gtitle'"`"
		flist="$flist `echo '-'``echo e` `echo "ytitle='$ytitle'"`"
		flist="$flist `echo '-'``echo e` `echo "xtitle='$xtitle'"`"

		flist="$flist `echo '-'``echo e` `echo interval=\'"$prefix".perf.$metric\'`"
		#flist="$flist `echo '-'``echo e` `echo buddystorel=\'"$prefix"_bl_60s.perf.time_$metric\'`"
		flist="$flist `echo '-'``echo e` `echo "oname='$output'"`"
	
		ymax=$( get_ymax $sz )
		flist="$flist `echo '-'``echo e` `echo "ymax='$ymax'"`"

		echo "gnuplot $flist 6_gnuplot_rados_latency.cfg"
		gnuplot $flist 4_gnuplot_timeline_perf_plot.cfg
		open "$output"
		
		rm $prefix.perf.$metric
	done
done

