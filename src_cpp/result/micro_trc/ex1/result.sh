OP="get put range_query"
CONF="uni zipf_1.2 zipf_1.4 zipf_1.6 zipf_1.8 zipf_2.0"
engine="!"
engine+=$(cat uni_get.rslt | awk '{print $1}')

echo $engine
for i in {1..10}; do
	cd ../ex$i
	for cf in $CONF; do
		for op in $OP; do
			test=$op			
			test+=$(cat ${cf}_$op.rslt | awk '{print $4}')
			echo $test
		done
	done	
done
