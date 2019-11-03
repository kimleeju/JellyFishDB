#ifndef JELLYFISH_SKIPLIST_H
#define JELLYFISH_SKIPLIST_H


#include "../SkipList.h"
//#include "Node.h"
#include "../Iterator.h"

/*struct JFNode : public Node {
    VNode* vqueue;
    int vqueue_num; 
};*/

class JellyFishSkipList : public SkipList{

private:
//    unordered_map<string, string> kv_map; // hash map 

public:
    int Put(string key, string value, Iterator iterator);
    string Get(string key, Iterator iterator); 
//    int remove(string key){};
public:
    void RangeQuery(string start_key, int count, Iterator iterator);
    Splice* AllocateSplice();
    Node* FindGreaterorEqual(const string& key);
	int RecomputeSpliceLevels(const string& key, int to_level, Splice* splice);
    int FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before);
    int KeyIsAfterNode(const string& key, Node* n);
    VNode* AllocateVNode(const string& value);
    Node* AllocateNode(const string& key, const string& value, int height); 
    bool Insert(string key, string value ,Iterator iterator); 
//	void SetThreadNum(int t_num){}		
	void PrintStat();
	void ResetStat();



public:
    JellyFishSkipList();
    ~JellyFishSkipList(){}
};

#endif
