skewness="uni zipf_1.2 zipf_1.4 zipf_1.6 zipf_1.8 zipf_2.0"
num="1 2 3 4 5"
operations="put get range_query"

echo "Engine BlockedSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList SimpleSkipList" > put.dat

for op in $operations; do
	echo "Engine BlockedSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList SimpleSkipList" > "$op".dat
	awk 'NR==1 {printf("%s %d %d %d %d %d\n", "uni", $1, $2, $3, $4, $5)}' ex5/"$op".rslt.dat >> "$op".dat
	awk 'NR==2 {printf("%s %d %d %d %d %d\n", "zipf_1.2", $1, $2, $3, $4, $5)}' ex5/"$op".rslt.dat >> "$op".dat
	awk 'NR==3 {printf("%s %d %d %d %d %d\n", "zipf_1.4", $1, $2, $3, $4, $5)}' ex5/"$op".rslt.dat >> "$op".dat
	awk 'NR==4 {printf("%s %d %d %d %d %d\n", "zipf_1.6", $1, $2, $3, $4, $5)}' ex5/"$op".rslt.dat >> "$op".dat
	awk 'NR==5 {printf("%s %d %d %d %d %d\n", "zipf_1.8", $1, $2, $3, $4, $5)}' ex5/"$op".rslt.dat >> "$op".dat
	awk 'NR==6 {printf("%s %d %d %d %d %d\n", "zipf_2.0", $1, $2, $3, $4, $5)}' ex5/"$op".rslt.dat >> "$op".dat
done
