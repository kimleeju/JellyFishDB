file="ex5/put.rslt ex5/get.rslt ex5/range_query.rslt"

for f in $file; do
	./2_change_row_col.sh $f
done
