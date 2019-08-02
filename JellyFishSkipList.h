#ifndef JELLYFISH_SKIPLIST_H
#define JELLYFISH_SKIPLIST_H

#include "SkipList.h"
#include "Node.h"

struct JFNode : public Node {
    VNode* vqueue;
    int vqueue_num; 
};

class JellyFishSkipList : public SkipList{

private:
    unordered_map<string, string> kv_map; // hash map 

public:
    int put(string key, string value){};
    string get(string key){}; 
    int remove(string key){}; 

public:
    JellyFishSkipList(){}
    ~JellyFishSkipList(){}
};

#endif