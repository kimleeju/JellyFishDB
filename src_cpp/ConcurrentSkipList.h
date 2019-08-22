#ifndef CONCURRENTSKIPLIST_H
#define CONCURRENTSKIPLIST_H

#include <list>
#include <cstring>
#include "SkipList.h"
#include "Iterator.h"


using namespace std;

class ConcurrentSkipList : public SkipList{

private:
   // unordered_map<string, string> kv_map; // hash map 

public:
    Node* Get(string key);
    void RangeQuery(string start_key, int count);
    Splice* AllocateSplice();
    Node* FindLast();
    Node* FindLessThan(string key, Node** prev);
    Node* FindGreaterorEqual(string key);
    void RecomputeSpliceLevels(Node* node, int level);
    void FindSpliceForLevel(Node* node, int level, Node** sp_prev, Node** sp_next, Node* before);
    bool KeyIsAfterNode(string key, Node* n);
    Node* AllocateKey();
    Node* AllocateNode(string key, string value, int height); 
    int RandomHeight();
    bool Insert(Node* nnode);
 

public:
    ConcurrentSkipList(int32_t max_height, int node_count);
    ~ConcurrentSkipList(){}
};

#endif
