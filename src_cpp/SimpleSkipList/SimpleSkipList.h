#ifndef SimpleSKIPLIST_H
#define SimpleSKIPLIST_H

#include <list>
#include <cstring>
#include <map>
#include <unordered_map>
#include <set>
#include "../SkipList.h"
#include "../Iterator.h"

using  namespace std;
class SimpleSkipList : public SkipList {

private:
    multimap<string, string> kv_map; // hash map 

public:
    int Put(string key, string value, Iterator iterator);
    string Get(string key, Iterator iterator); 
//    int remove(string key); 

   
public:
    void RangeQuery(string start_key, int count, Iterator iterator);
    Splice* AllocateSplice(){return nullptr;}
    Node* FindGreaterorEqual(const string& key){return nullptr;}
    int RecomputeSpliceLevels(const string& key, int to_level, Splice* splice = 0){return 0;}
    int FindSpliceForLevel(const string& key,int to_level,Node** sp_prev, Node** sp_next, Node* before){}
    int KeyIsAfterNode(const string& key, Node* n){return 0;}
    Node* AllocateNode(const string& key, const string& value, int height){return nullptr;} 
    bool Insert(string key, string value, Iterator iterator);
	void PrintStat();
	void ResetStat();
public:
    SimpleSkipList();
    ~SimpleSkipList(){}
};

#endif
