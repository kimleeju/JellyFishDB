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
#include "Arena.h"
#include "Node.h"
#include "LinkedSkipList/Node_Linked.h"
#include "Util.h"
#include "BloomFilter.h"
using namespace std;
#define MAX_LEVEL 12

#define DEBUG(x) do {\
	if(0) { std::cout << x << std::endl; } \
} while (0)

#define SET_LEVEL(x) do{\
	if(0) {SET_LEVEL[x]++;}\
} while(0)

#define PUT_LEVEL(x) do{\
	if(0) {PUT_LEVEL[x]++;}\
} while(0)

#define GET_LEVEL(x) do{\
	if(0) {GET_LEVEL[x]++;}\
} while(0)

#define GET_REFERENCE(x) do{\
	if(0) {GET_REFERENCE[x]++;}\
} while(0)

#define PUT_REFERENCE(x) do{\
	if(0) {PUT_REFERENCE[x]++;}\
} while(0)

#define COUNT(x) do{\
	if(0) {x++;} \
} while (0)



#define OP_EXEC
//#define PRINT_LATENCY
//#define PRINT_HEIGHT
//#define PRINT_STAT 
//#define PRINT_LEVEL
//#define PRINT_SET_LEVEL
//#define PRINT_REFERENCE
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
	BloomFilter bloom;    
	Node* head_;
	atomic<int> cnt;
	atomic<int> pointer_cnt;
	atomic<int> CAS_failure_cnt;
	atomic<int> CAS_cnt;
	atomic<int> PUT_LEVEL[MAX_LEVEL];
	atomic<int> GET_LEVEL[MAX_LEVEL];
	atomic<int> SET_LEVEL[MAX_LEVEL];
	atomic<int> GET_REFERENCE[MAX_LEVEL];
	atomic<int> PUT_REFERENCE[MAX_LEVEL];
protected:
	uint16_t kMaxHeight_;
	Splice* seq_splice;
	TimeStamp t_global_committed;
	atomic<long> cpr_cnt; 	    
	atomic<int>max_height_;
public:    
    //string randomString();
	virtual int Put(string key, string value, Iterator& iterator ) = 0;
	virtual string Get(string key, Iterator iterator) = 0;
	virtual void RangeQuery(string start_key, int count, Iterator iterator) = 0;

	virtual Splice* AllocateSplice() =0;
	virtual Node* FindGreaterorEqual(const string& key)=0;
	virtual int RecomputeSpliceLevels(const string& key, int to_level, Splice* splice = 0)=0;
	virtual int FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before)=0;
	virtual int KeyIsAfterNode(const string& key, Node* n)=0;
	virtual Node* AllocateNode(const string& key, const string& value, int height)=0;
 	//virtual int RandomHeight()=0;
    	virtual bool Insert(string key, string value, Iterator& iterator)=0;
	virtual void SetThreadNum(int t_num){}		
	virtual void PrintReference(){}
	virtual void PrintSetLevel(){}
	virtual void PrintLevel(){}
	virtual void PrintStat(){}
	virtual void ResetStat(){}
	
public:
	int RandomHeight(){
		int height = 1;

		int rnum = rand();
		//if(!(rnum & 0x3)) { // 둘다 0 이어야 동작. 
		if(1) {
			while(!(rnum & 0x3)  && height < kMaxHeight_) {
				height++;
				rnum = rand();
				//rnum <<= 1;
			} 
		}
		return height;
	};

    SkipList(int kMaxHeight, Node* head, Splice* seq_splice_) : 
        head_(head),
        kMaxHeight_(kMaxHeight),
        seq_splice(seq_splice_)
    {}

    SkipList()  
    {}
    ~SkipList() {}  
};
#endif
