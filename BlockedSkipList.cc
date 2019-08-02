#include "BlockedSkipList.h"

void BlockedSkipList::init()
{
//    sl
    cout << __func__ << endl;
    return; 
}

int BlockedSkipList::put(string key, string value)
{
    cout << __func__ << endl; 
    std::lock_guard<std::mutex> lock(sl_mutex);
    // put data 
    std::lock_guard<std::mutex> unlock(sl_mutex);
    return 1;
}

string BlockedSkipList::get(string key) 
{ 
    cout << __func__ << endl; 
    string val = kv_map.find(key)->second;
    return val;
}

int BlockedSkipList::remove(string key)
{
    cout << __func__ << endl; 
    return 1;
}
