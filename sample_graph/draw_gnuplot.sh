
if [[ $# -lt 1 ]]; then
	echo "./.sh [response | cdf]"
	echo "E.g.: ./sh response"
	exit
fi

prefix=$1
config_file=$prefix.cfg
output_file=$prefix.eps
# datatilfe=response.cfg
#gnuplot response.cfg
#open response.eps

gnuplot $config_file
open $output_file
