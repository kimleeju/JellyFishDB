#include "SimpleSkipList.h"

int SimpleSkipList::Put(string key, string value, Iterator iterator){
    t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
	iterator.Put(key,value, iterator);
    t_global_committed.mlock.unlock();
    return 0;
}


string SimpleSkipList::Get(string key , Iterator iterator){
    t_global_committed.mlock.lock();
	t_global_committed.get_and_inc();
    multimap<string, string>::iterator iter;
	iter = kv_map.find(key);
	if (iter == kv_map.end()) { // not found
		t_global_committed.mlock.unlock();
		return "not found";
	}
	else { // found
		string val = iter->second;
		t_global_committed.mlock.unlock();
		return val;
	}
}
    


void SimpleSkipList::RangeQuery(string start_key, int count, Iterator iterator ){
    t_global_committed.mlock.lock();
	t_global_committed.get_and_inc();

    multimap<string, string> m;
	multimap<string, string>::iterator iter;
	int cnt = count;

	iter = kv_map.find(start_key);
	
	if (iter != kv_map.end()) {
		for (; (iter != kv_map.end()) && (cnt > 0); iter++, cnt--) {
			m.insert(make_pair(iter->first, iter->second)); 
		}
	}
	else { }
	t_global_committed.mlock.unlock();
}


bool SimpleSkipList::Insert(string key, string value, Iterator iterator){
	kv_map.insert(make_pair(key, value));
	return true;
}

void SimpleSkipList::PrintStat()
{
	cout << "SimpleSkipList comparator count = " << cpr_cnt << endl;

}
void SimpleSkipList::ResetStat()
{
	cpr_cnt = 0;
}

SimpleSkipList::SimpleSkipList()
{
    srand((unsigned)time(NULL));
	cpr_cnt = 0;
}
