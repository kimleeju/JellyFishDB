prev=$1
after=$2
find . -name "*$1*" | sed -e 'p' -e "s/$prev/$after/g" | xargs -n 2 mv
#find . -name "res*" | sed -e 'p' -e 's/res/flush/g' | xargs -n 2 mv
