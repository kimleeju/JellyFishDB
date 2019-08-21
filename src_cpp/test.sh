#!/bin/bash

benchmarks="a b c d e f"
threads="1 2 4 8 16"
DIR="../trc/result/extract_rslt"

for bench in $benchmarks
do
	for th in $threads
	do
		# threadnum op_num benchmark path
		./Run SimpleSkipList $th $DIR/merge_"$bench"_th"$th".log > ../trc/result/"$bench"_th"$th".rslt
	done
done

for b in $benchmarks
do
	for th in $threads
	do
		cat ../trc/result/"$b"_th"$th".rslt >> ../trc/result/"$b".rslt
	done
done
