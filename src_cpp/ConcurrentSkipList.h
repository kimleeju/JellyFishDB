#ifndef CONCURRENTSKIPLIST_H
#define CONCURRENTSKIPLIST_H

#include "SkipList.h"

class ConcurrentSkipList : public SkipList{

private:
    unordered_map<string, string> kv_map; // hash map 

public:
    int put(string key, string value);
    string get(string key); 
    int remove(string key); 

public:
    ConcurrentSkipList(){}
    ~ConcurrentSkipList(){}
};

#endif