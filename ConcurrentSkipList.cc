#include "ConcurrentSkipList.h"

int ConcurrentSkipList::put(string key, string value)
{
    cout << __func__ << endl; 
    return 1;
}

string ConcurrentSkipList::get(string key) 
{ 
    cout << __func__ << endl; 
    string val = "hello";
    return val;
}

int ConcurrentSkipList::remove(string key)
{
    cout << __func__ << endl; 
    return 1;
}