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
#include<assert.h>
//#include<memory>
// #include<pthread.h>

#include "Node.h"
#include "Util.h"

using namespace std;
#define MAX_LEVEL 32

#define DEBUG(x) do {\
	if(0) { std::cerr << x << std::endl; } \
} while (0)

#define OP_EXEC
//#define PRINT_LATENCY
//#define PRINT_HEIGHT
#define PRINT_STAT
#define PRINT_PERF
#define JELLYFISH



class Iterator;

class SkipList {
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
	atomic<long> cpr_cnt; 	    

public:    
    //string randomString();
    virtual int Put(string key, string value, Iterator iterator ) = 0;
    virtual string Get(string key, Iterator iterator) = 0;
    virtual void RangeQuery(string start_key, int count, Iterator iterator) = 0;

    virtual Splice* AllocateSplice() =0;
    virtual Node* FindLast() =0;
    virtual Node* FindLessThan(const string& key, Node** prev)=0;
    virtual Node* FindGreaterorEqual(const string& key)=0;
    virtual int RecomputeSpliceLevels(const string& key, int to_level, Splice* splice = 0)=0;
    virtual void FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before)=0;
    virtual bool KeyIsAfterNode(const string& key, Node* n)=0;
    virtual Node* AllocateNode(const string& key, const string& value, int height)=0;
    //virtual int RandomHeight()=0;
    virtual bool Insert(string key, string value, Iterator iterator)=0;
	virtual void SetThreadNum(int t_num){}		
	virtual void PrintStat(){}
	virtual void ResetStat(){}
	
public:
	int RandomHeight(){
		int height = 1;
		int rnum = rand();

		if(rnum & 0x3) { // 둘다 0 이어야 동작. 
			while(rnum & 1 << 30 && height < kMaxHeight_) {
				height++;
				rnum <<= 1;
			} 
		}
		return height;
	};

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
