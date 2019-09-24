operations="put get range_query"
exercise="ex1 ex2 ex3 ex4 ex5 ex6 ex7 ex8 ex9 ex10"
skewness="uni zipf_1.2 zipf_1.4 zipf_1.6 zipf_1.8 zipf_2.0"
#skiplist="BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList SimpleSkipList JellyFishSkipList"
skiplist="2 3 4 5 6 7"

for op in $operations; do
	for ex in $exercise; do
		for sk in $skewness; do
			echo "" > "$sk"_"$op".rslt 
			for sl in $skiplist; do
				if [[ $ex == "ex1" ]]; then	
					#echo "head -n "$sl" ../"$ex"/"$sk"_"$op".rslt | tail -n 1 | awk '{print $4}' > "$sl"_"$sk"_"$op".log"
					head -n "$sl" ../"$ex"/"$sk"_"$op".rslt | tail -n 1 | awk '{print $4}' > "$sl"_"$sk"_"$op".log
				else
					#echo "head -n "$sl" ../"$ex"/"$sk"_"$op".rslt | tail -n 1 | awk '{print $4}' >> "$sl"_"$sk"_"$op".log"
					head -n "$sl" ../"$ex"/"$sk"_"$op".rslt | tail -n 1 | awk '{print $4}' >> "$sl"_"$sk"_"$op".log
				fi
				#rm -rf "$sl"_"$sk"_"$op".log
				#rm -rf "$sl"_"$sk"_"$op".rslt
			done
			for sl in $skiplist; do
				sort -nr "$sl"_"$sk"_"$op".log > "$sl"_"$sk"_"$op".rslt
				head -n 5 "$sl"_"$sk"_"$op".rslt | tail -n 1 >> "$sk"_"$op".rslt 
				rm -rf "$sl"_"$sk"_"$op".log
				rm -rf "$sl"_"$sk"_"$op".rslt
			done			
				
		done		
	done
done

#for in $operations; do
	
#done
