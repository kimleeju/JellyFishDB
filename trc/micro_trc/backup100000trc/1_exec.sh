cat zipf_1.2_range_query_load.trc | awk '{print $2}' | sort | uniq -c| awk {'print $1'} | head -n 10
#cat dup_2_range_query_run.trc| awk '{print $2}' | sort | uniq -c|  head -n 10
