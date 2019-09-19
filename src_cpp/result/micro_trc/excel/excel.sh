operations="put get range_query"


for op in $operations; do
	paste ../uni_"$op".rslt ../zipf_1.2_"$op".rslt ../zipf_1.4_"$op".rslt ../zipf_1.6_"$op".rslt ../zipf_1.8_"$op".rslt ../zipf_2.0_"$op".rslt > "$op".rslt
done
