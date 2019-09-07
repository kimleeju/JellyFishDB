#ifndef SimpleSKIPLIST_H
#define SimpleSKIPLIST_H

#include "SkipList.h"

class SimpleSkipList : public SkipList{

private:
    unordered_map<string, string> kv_map; // hash map 

public:
    int put(string key, string value);
    string get(string key); 
    int remove(string key); 

public:
    SimpleSkipList(){}
    ~SimpleSkipList(){}
};

#endif