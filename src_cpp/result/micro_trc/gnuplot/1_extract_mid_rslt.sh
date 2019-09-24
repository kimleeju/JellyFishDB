skewness="uni zipf_1.2 zipf_1.4 zipf_1.6 zipf_1.8 zipf_2.0"
operations="put get range_query"

###extract IOPS'
for sk in $skewness; do
	for op in $operations; do
		#awk '{printf("%d\n", $4)}' ../ex/"$sk"_"$op".log > ex5/"$sk"_"$op".rslt
		cat ../ex/"$sk"_"$op".rslt > ex5/"$sk"_"$op".log
	done
done

###combine result of each put get range_query
for op in $operations; do
	paste ex5/uni_"$op".log ex5/zipf_1.2_"$op".log ex5/zipf_1.4_"$op".log ex5/zipf_1.6_"$op".log ex5/zipf_1.8_"$op".log ex5/zipf_2.0_"$op".log > ex5/"$op".rslt
	#sed -i '1d' ex5/"$op".log
done

rm -rf ex5/*.log 
