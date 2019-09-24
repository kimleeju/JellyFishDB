
#ifndef SKIPLIST_H
#define SKIPLIST_H
#include<atomic>
#include<string>
#include<iostream>
#include<map>
#include<unordered_map>
#include<set>
#include<mutex>
#include<cstring>
//#include<memory>
// #include<pthread.h>

#include "Node.h"
#include "Util.h"

using namespace std;
#define MAX_LEVEL 12 

class Iterator;

class SkipList {
//protected:
public:

    class Splice{
        public:
        int height_ =0;
        Node** prev_;
        Node** next_;
    };
    Node* head_;
	atomic<int> cnt;
protected:
    uint16_t kMaxHeight_;
    atomic<int>max_height_; 
    Splice* seq_splice;
    TimeStamp t_global_committed;
    
public:    
    //string randomString();
    virtual int Put(string key, string value, Iterator iterator ) = 0;
    virtual string Get(string key, Iterator iterator) = 0;
    virtual void RangeQuery(string start_key, int count, Iterator iterator) = 0;
    virtual Splice* AllocateSplice() =0;
    virtual Node* FindLast() =0;
    virtual Node* FindLessThan(string key, Node** prev)=0;
    virtual Node* FindGreaterorEqual(string key)=0;
    virtual int RecomputeSpliceLevels(string key, int level, Splice* splice = 0)=0;
    virtual void FindSpliceForLevel(string key, int level, int cur_level,Node** sp_prev, Node** sp_next, Node* before)=0;
    virtual bool KeyIsAfterNode(string key, Node* n)=0;
    virtual Node* AllocateNode(string key, string value, int height)=0;
    virtual int RandomHeight()=0;
    virtual bool Insert(string key, string value ,Iterator iterator)=0;
	virtual void GetEnv(int t_num){
		return;
	}		
	
    /** 
     * put, get, delete 
     * */
//    virtual int put(string key, string value) = 0;
//    virtual string get(string key) = 0;
//    virtual int remove (string key) = 0;
    // virtual set<string, string> range_query (string starting_key, int n) = 0;

public:
    SkipList(int kMaxHeight, Node* head, int max_height, Splice* seq_splice_) : 
        head_(head),
        kMaxHeight_(kMaxHeight),
        max_height_(max_height),
        seq_splice(seq_splice_)
    {}

    SkipList()  
    {}
    ~SkipList() {}  
};
#endif
