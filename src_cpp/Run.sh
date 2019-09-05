SkipList_Name=("BlockedSKipList"  "ConcurrentSkipList"  "JellyFishSkipList")
Thread_num=("1" "2" "4")
path=../trc/micro_trc/micro_a_th4.log

./Run ${SkipList_Name[2]} 4 path
#./Run ${SkipList_Name[2]} 4 ../trc/micro_trc/micro_a_th4.log

