#ifndef BLOCKED_SKIPLIST_H
#define BLOCKED_SKIPLIST_H

#include <list>
#include <cstring>
#include "../SkipList.h"
#include "../Iterator.h"


 

using namespace std;

class BlockedSkipList : public SkipList{

public:
    int Put(string key, string value, Iterator iterator);
    string Get(string key, Iterator iterator); 
//    int remove(string key); 

   
public:
    void RangeQuery(string start_key, int count, Iterator iterator);
    Splice* AllocateSplice();
    Node* FindLast();
    Node* FindLessThan(string key, Node** prev);
    Node* FindGreaterorEqual(string key);
    int RecomputeSpliceLevels(string key, int level, Splice* splice = 0);
    void FindSpliceForLevel(string key, int level, int cur_level, Node** sp_prev, Node** sp_next, Node* before);
    int Comparator(string key1, string key2);
	bool KeyIsAfterNode(string key, Node* n);
    Node* AllocateNode(string key, string value, int height); 
    int RandomHeight();
    bool Insert(string key, string value, Iterator iterator);
	void GetEnv(int t_num);
public:
    BlockedSkipList();
    ~BlockedSkipList(){}
};

#endif
