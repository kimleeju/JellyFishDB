#ifndef BLOCKED_SKIPLIST_H
#define BLOCKED_SKIPLIST_H

#include <list>
#include <cstring>
#include "SkipList.h"
#include "Iterator.h"


 

using namespace std;

class BlockedSkipList : public SkipList{

public:
    int put(string key, string value, Iterator iterator);
    string get(string key, Iterator iterator); 
//    int remove(string key); 

   
public:
    void RangeQuery(string start_key, int count, Iterator iterator);
    Splice* AllocateSplice();
    Node* FindLast();
    Node* FindLessThan(string key, Node** prev);
    Node* FindGreaterorEqual(string key);
    void RecomputeSpliceLevels(string key, int level, Splice* splice = 0);
    void FindSpliceForLevel(string key, int level, Node** sp_prev, Node** sp_next, Node* before);
    bool KeyIsAfterNode(string key, Node* n);
    Node* AllocateNode(string key, string value, int height); 
    int RandomHeight();
    bool Insert(string key, string value, Iterator iterator);
public:
    BlockedSkipList();
    ~BlockedSkipList(){}
};

#endif
