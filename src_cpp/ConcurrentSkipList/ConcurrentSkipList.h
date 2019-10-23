#ifndef CONCURRENTSKIPLIST_H
#define CONCURRENTSKIPLIST_H

#include <list>
#include <cstring>
#include <assert.h>
#include "../SkipList.h"
#include "../Iterator.h"


using namespace std;

class ConcurrentSkipList : public SkipList{

private:
	long cpr_cnt;
public:
	// unordered_map<string, string> kv_map; // hash map 
	int Put(string key, string value, Iterator iterator);
	string Get(string key, Iterator iterator);
	void RangeQuery(string start_key, int count, Iterator iterator);

public:
    Splice* AllocateSplice();
    Node* FindLast();
    Node* FindLessThan(const string& key, Node** prev);
    Node* FindGreaterorEqual(const string& key);

	void FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before);
    int RecomputeSpliceLevels(const string& key, int to_level, Splice* splice);
	int Comparator(string key1, string key2);
    bool KeyIsAfterNode(const string& key, Node* n);
    Node* AllocateNode(const string& key, const string& value, int height); 
    bool Insert(string key, string value , Iterator iterator);
	void PrintStat();
	void ResetStat();

public:
    ConcurrentSkipList();
    ~ConcurrentSkipList(){}
};

#endif
