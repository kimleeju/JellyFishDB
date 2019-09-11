
if [[ $# -lt 1 ]]; then
	echo "./.sh [ardb]"
	echo "E.g.: ./sh ardb"
	exit
fi

prefix=$1
config_file=$prefix.cfg
output_file=$prefix.eps
# datatilfe=response.cfg
#gnuplot response.cfg
#open response.eps

gnuplot $config_file
xdg-open $output_file
