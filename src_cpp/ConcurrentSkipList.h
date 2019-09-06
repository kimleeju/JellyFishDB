#ifndef CONCURRENTSKIPLIST_H
#define CONCURRENTSKIPLIST_H

#include <list>
#include <cstring>
#include "SkipList.h"
#include "Iterator.h"


using namespace std;

class ConcurrentSkipList : public SkipList{

public:
   // unordered_map<string, string> kv_map; // hash map 
   int put(string key, string value, Iterator iterator);
   string get(string key, Iterator iterator);

public:
    void RangeQuery(string start_key, int count, Iterator iterator);
    Splice* AllocateSplice();
    Node* FindLast();
    Node* FindLessThan(string key, Node** prev);
    Node* FindGreaterorEqual(string key);
    int RecomputeSpliceLevels(string key, int level, Splice* splice);
    void FindSpliceForLevel(string key, int level, Node** sp_prev, Node** sp_next, Node* before);
    bool KeyIsAfterNode(string key, Node* n);
    Node* AllocateNode(string key, string value, int height); 
    int RandomHeight();
    bool Insert(string key, string value , Iterator iterator);
 

public:
    ConcurrentSkipList();
    ~ConcurrentSkipList(){}
};

#endif
