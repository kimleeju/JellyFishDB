operations="put get range_query"

for op in $operations; do
	echo "gnuplot `$op`.cfg ..."
	gnuplot "$op".cfg
	echo "xdg-open `$op`.eps ..."
	xdg-open "$op".eps
done
