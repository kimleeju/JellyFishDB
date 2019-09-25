#ifndef STRIDE_SKIPLIST_H
#define STRIDE_SKIPLIST_H

#include <list>
#include <cstring>
#include "../SkipList.h"
#include "../Iterator.h"


using namespace std;

class StrideSkipList : public SkipList{

public:
   // unordered_map<string, string> kv_map; // hash map 
   int Put(string key, string value, Iterator iterator);
   string Get(string key, Iterator iterator);

public:
    void RangeQuery(string start_key, int count, Iterator iterator);
    Splice* AllocateSplice();
    Node* FindLast();
    Node* FindLessThan(string key, Node** prev);
    Node* FindGreaterorEqual(string key);
    int RecomputeSpliceLevels(string key, int level, Splice* splice);
    void FindSpliceForLevel(string key, int level, int cur_level, Node** sp_prev, Node** sp_next, Node* before);
	int Comparator(string key1, string key2);
    bool KeyIsAfterNode(string key, Node* n);
    Node* AllocateNode(string key, string value, int height); 
    int RandomHeight();
    bool Insert(string key, string value , Iterator iterator);
 

public:
    StrideSkipList();
    ~StrideSkipList(){}
};

#endif
