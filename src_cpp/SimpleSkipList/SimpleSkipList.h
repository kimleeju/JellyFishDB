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
    Node* FindLast(){return nullptr;}
    Node* FindLessThan(string key, Node** prev){return nullptr;}
    Node* FindGreaterorEqual(string key){return nullptr;}
    int RecomputeSpliceLevels(string key, int level, Splice* splice = 0){return 0;}
    void FindSpliceForLevel(string key, int level, int cur_level,Node** sp_prev, Node** sp_next, Node* before){}
    bool KeyIsAfterNode(string key, Node* n){return 0;}
    Node* AllocateNode(string key, string value, int height){return nullptr;} 
    int RandomHeight(){return 0;}
    bool Insert(string key, string value, Iterator iterator);
public:
    SimpleSkipList(){}
    ~SimpleSkipList(){}
};

#endif
