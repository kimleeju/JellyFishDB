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
    Node* FindLessThan(const string& key, Node** prev);
    Node* FindGreaterorEqual(const string& key);
    int RecomputeSpliceLevels(const string& key, int to_level, Splice* splice);
    void FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before);
    bool KeyIsAfterNode(const string& key, Node* n);
    Node* AllocateNode(const string& key, const string& value, int height); 
	int Comparator(string key1, string key2);
    bool Insert(string key, string value , Iterator iterator);
 	void PrintStat();
	void ResetStat();

public:
    StrideSkipList();
    ~StrideSkipList(){}
};

#endif
