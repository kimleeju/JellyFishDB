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
    Node* FindLessThan(const string& key, Node** prev);
    Node* FindGreaterorEqual(const string& key);
    int RecomputeSpliceLevels(const string& key, int to_level, Splice* splice = 0);
    void FindSpliceForLevel(const string& key, int to_level, Node** sp_prev, Node** sp_next, Node* before);
    int Comparator(string key1, string key2);
	bool KeyIsAfterNode(const string& key, Node* n);
    Node* AllocateNode(const string& key, const string& value, int height); 
    int RandomHeight();
    bool Insert(string key, string value, Iterator iterator);
	void GetEnv(int t_num);
public:
    BlockedSkipList();
    ~BlockedSkipList(){}
};

#endif
