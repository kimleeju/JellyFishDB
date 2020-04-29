#ifndef STRIDE_SKIPLIST_H
#define STRIDE_SKIPLIST_H

#include <list>
#include <cstring>
#include <assert.h>
#include "../SkipList.h"
#include "../Iterator.h"


using namespace std;

class StrideSkipList : public SkipList{

public:
   // unordered_map<string, string> kv_map; // hash map 
	int Put(string key, string value, Iterator& iterator);
	string Get(string key, Iterator iterator);
	void RangeQuery(string start_key, int count, Iterator iterator);
public:
	Splice* AllocateSplice();
	Node* FindGreaterorEqual(const string& key);
	int RecomputeSpliceLevels(const string& key, int to_level, Splice* splice);
	int FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before);
	int KeyIsAfterNode(const string& key, Node* n);
	Node* AllocateNode(Iterator& iterator,const string& key, const string& value, int height); 
	Node* AllocateNode(const string& key, const string& value, int height); 
	bool Insert(string key, string value , Iterator& iterator);
	void PrintReference();	
	void PrintSetLevel();
	void PrintLevel();
	void PrintStat();
	void ResetStat();


public:
	StrideSkipList();
	~StrideSkipList(){}
};

#endif
