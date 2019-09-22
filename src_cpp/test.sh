make clean
make
op=$1
op=100

#./Run BlockedCVSkipList 1 ./bb.trc ./bb.trc
./Run BlockedSpinSkipList 1 ./aa.trc ./aa.trc
#./Run BlockedCVSkipList 1 ./aa.trc ./aa.trc
#./Run BlockedCVSkipList 16 ../trc/micro_trc/backup"$op"trc/result/uni_get_load.trc ../trc/micro_trc/backup"$op"trc/result/uni_get_run.trc
