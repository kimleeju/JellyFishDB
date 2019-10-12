
set xlabel "IOPS" font ", 22"
set xrange [1:4]
set yrange [0:800000]
plot for [col=2:6] 'ss.dat' using 1:col with linespoints title columnheader
