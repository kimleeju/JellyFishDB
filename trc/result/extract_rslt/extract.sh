### 1. copy file : 1~100000 line
workloads="a b c d f"
threads="1 2 4 8 16"
DIR=../orig_trc

for th in $threads; do
	head -n 100000 $DIR/ycsb_load_a_th"$th".log > ycsb_load_a_th"$th".tmp
	head -n 100000 $DIR/ycsb_load_e_th"$th".log > ycsb_load_e_th"$th".tmp
done

for work in $workloads; do
	for th in $threads; do
		head -n 100000 $DIR/ycsb_run_"$work"_th"$th".log > ycsb_run_"$work"_th"$th".tmp
	done
done

for th in $threads; do
	head -n 100000 $DIR/ycsb_run_e_th"$th".log > ycsb_run_e_th"$th".tmp
done

### 2. merge load_a + {run a, b, c, d, f}

#mv $DIR/temp.log $DIR/ycsb_load_a.log

for w in $workloads; do
	for th in $threads; do
		fname="ycsb_run_""$w""_th"$th.tmp
		lfname="ycsb_load_a""_th"$th.tmp
		echo "merge $lfname + "$fname" ..."
		cat $lfname > merge_"$w"_th"$th".log
		cat $fname >> merge_"$w"_th"$th".log 
	done

done

for th in $threads; do
	echo "merge ycsb_load_e_th"$th".tmp + ycsb_run_th"$th".tmp ..."
	cat ycsb_load_e_th"$th".tmp > merge_e_th"$th".log
	cat ycsb_run_e_th"$th".tmp >> merge_e_th"$th".log
done

### 2. filtering 
#
#for w in $workloads; do
#	for th in $threads; do
#		fname="merge_""$w"_th"$th".log
#		echo "filter $fname ..."
#		rfname="count_""$w"_th"$th".log
#	        awk '$2=="put" || $2=="update" {print $3}' $fname | sort | uniq -c | sort -nr | cat -b | sed -e '1 i\#x count key ' > $rfname	
#	       #awk '$1=="put" || $1=="update" {print $2}' $fname | sort | uniq -c | sort -nr | awk '{print NR "\t" $0}' > $rfname 
#        done
#done
#
#for th in $threads; do
#	echo "filter merge_e_th"$th".log ..."
#	awk '$2=="put" || $2=="update" {print $3}' merge_e_th"$th".log | sort | uniq -c | sort -nr | cat -b | sed -e '1 i\#x count key' > count_e_th"$th".log	
#	#awk '$1=="put" || $1=="update" {print $2}' merge_e.log | sort | uniq -c | sort -nr | awk '{print NR "\t" $0}' > count_e.log
#done
