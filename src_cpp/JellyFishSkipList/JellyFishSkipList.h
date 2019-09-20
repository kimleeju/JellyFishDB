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
    Node* FindLast();
    Node* FindLessThan(string key, Node** prev);
    Node* FindEqual(string key);
    Node* FindGreaterorEqual(string key);
    int RecomputeSpliceLevels(string key, int level, Splice* splice);
    void FindSpliceForLevel(string key, int level, Node** sp_prev, Node** sp_next, Node* before);
	int Comparator(string key1, string key2); 
     bool KeyIsAfterNode(string key, Node* n);
    VNode* AllocateVNode(string value);
    Node* AllocateNode(string key, string value, int height); 
    int RandomHeight();
    bool Insert(string key, string value ,Iterator iterator); 

public:
    JellyFishSkipList();
    ~JellyFishSkipList(){}
};

#endif
