

for f in *.nkey; do
	echo "$f ..."
	python3.7 ./4_stat.py $f
done
