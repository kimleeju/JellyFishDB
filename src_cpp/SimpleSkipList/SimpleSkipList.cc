#include "SimpleSkipList.h"

int SimpleSkipList::put(string key, string value)
{
    cout << __func__ << endl; 
    kv_map.insert(make_pair(key, value));
    return 1;
}

string SimpleSkipList::get(string key) 
{ 
    cout << __func__ << endl; 
    string val = kv_map.find(key)->second;
    return val;
}

int SimpleSkipList::remove(string key)
{
    cout << __func__ << endl; 
    return 1;
}


