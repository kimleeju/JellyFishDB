skewness="uni zipf_1.2 zipf_1.4 zipf_1.6 zipf_1.8 zipf_2.0"
operations="put get range_query"

###extract IOPS
for sk in $skewness; do
	for op in $operations; do
		awk '{printf("%d	\n", $4)}' ../"$sk"_"$op".rslt > "$sk"_"$op".rslt
	done
done

###combine result of each put get range_query
for op in $operations; do
	paste uni_"$op".rslt zipf_1.2_"$op".rslt zipf_1.4_"$op".rslt zipf_1.6_"$op".rslt zipf_1.8_"$op".rslt zipf_2.0_"$op".rslt > "$op".rslt
	sed -i '1d' "$op".rslt
done
