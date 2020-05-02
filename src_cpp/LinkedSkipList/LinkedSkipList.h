#ifndef LINKEDSKIPLIST_H
#define LINKEDSKIPLIST_H

#include <list>
#include <cstring>
#include <assert.h>
#include "SkipList_Linked.h"
#include "Iterator_Linked.h"


using namespace std;

class LinkedSkipList : public SkipList_Linked{

private:
	long cpr_cnt;
public:
	// unordered_map<string, string> kv_map; // hash map 
	int Put(string key, string value, Iterator_Linked& iterator);
	string Get(string key, Iterator_Linked iterator);
	void RangeQuery(string start_key, int count, Iterator_Linked iterator);

public:
	Splice* AllocateSplice();
	Node_Linked* FindGreaterorEqual(const string& key);
	int FindSpliceForLevel(const string& key, int level, Node_Linked** sp_prev, Node_Linked** sp_next, Node_Linked* before);
	int RecomputeSpliceLevels(const string& key, int to_level, Splice* splice);
	int KeyIsAfterNode(const string& key, Node_Linked* n);
	VNode_Linked* AllocateVNode(Iterator_Linked& iterator, const string& value);
	Node_Linked* AllocateNode(Iterator_Linked& iterator,const string& key, const string& value, int height); 
	Node_Linked* AllocateNode(const string& key, const string& value, int height); 
	bool Insert(string key, string value , Iterator_Linked &iterator);
	void PrintReference();	
	void PrintSetLevel();
	void PrintLevel();
	void PrintStat();
	void ResetStat();

public:
	LinkedSkipList();
	~LinkedSkipList(){}
};

#endif
