#ifndef BLOCKED_SKIPLIST_H
#define BLOCKED_SKIPLIST_H

#include <list>
#include "SkipList.h"
#include "Node.h"

using namespace std;

class BlockedSkipList : public SkipList{

private:
    unordered_map<string, string> kv_map; // hash map 
    list<Node> sl; 
    mutex sl_mutex; 

public:
    void init();
    int put(string key, string value){};
    string get(string key){}; 
    int remove(string key){}; 

public:
    BlockedSkipList(){}
    ~BlockedSkipList(){}
};

#endif
