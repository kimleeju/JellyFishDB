#ifndef NODE_LINKED_H
#define NODE_LINKED_H

#include<string>
#include<vector>
#include<queue>
#include "../SkipList.h"
#include "../Util.h"
using namespace std;



// VNode_Linked 
typedef struct VNode_Linked{
	VNode_Linked* NoBarrier_Next() {
		return (&next)->load(std::memory_order_relaxed);
	}

	void NoBarrier_SetNext(VNode_Linked* x) {
		(&next)->store(x,std::memory_order_relaxed);	
	}
	
	bool CASNext(VNode_Linked* expected, VNode_Linked* x) {
		return (&next)->compare_exchange_strong(expected, x);
	}

	string value; 
	string timestamp; 
	atomic<VNode_Linked*> next; 

}VNode_Linked;


// Node_Linked 
typedef struct Node_Linked{
public:

	Node_Linked* Next(int n) {
		//return (&prefix[0]-sizeof(Node_Linked*)*(n))->load(std::memory_order_acquire);
		return (&prefix[n])->load(std::memory_order_acquire);
	}

	void SetNext(int n, Node_Linked* x) {
		(&prefix[n])->store(x, std::memory_order_release);	
	}

	bool CASNext(int n, Node_Linked* expected, Node_Linked* x) {
		return (&prefix[n])->compare_exchange_strong(expected, x);	
	}


	Node_Linked* NoBarrier_Next(int n) {
		return (&prefix[n])->load(std::memory_order_relaxed);
	}
				
	void NoBarrier_SetNext(int n, Node_Linked* x) {
		(&prefix[n])->store(x,std::memory_order_relaxed);	
	}

	void InsertAfter(Node_Linked* prev, int level) {
		NoBarrier_SetNext(level, prev->NoBarrier_Next(level));
		prev->SetNext(level, this);
	}

	void Set_key(string key){
		str_key = key;
	}

	string Get_key(){
		string temp(str_key);
		return temp;
	}

	void Set_value(string value){
		str_value = value;
	}

	string Get_value(){
		return str_value;
	}
				
	void Set_height(int height_){
		height = height_;
	}

	int Get_height(){
		return height;
	}

//#ifdef JELLYFISH_SKIPLIST_H
	atomic<VNode_Linked*> vqueue;
//	atomic<int> vqueue_num;


	bool CAS_vqueue(VNode_Linked* expected, VNode_Linked* n){
		return (&vqueue)->compare_exchange_strong(expected,n);
	}

	void Set_vqueue(VNode_Linked* n){
		(&vqueue)->store(n, std::memory_order_relaxed);
	}

	VNode_Linked* Get_vqueue(){
		return vqueue;
	}
#if 0
	void Set_vqueue_num(){
		vqueue_num++;
	}
				
	int Get_vqueue_num(){
		return vqueue_num;
	}
#endif
//#endif

//#ifdef STRIDE_SKIPLIST_H
	void Set_stride_next(Node_Linked* next){
		stride_next = next;
	}

	Node_Linked* Get_stride_next(){
		return stride_next;
	}
//#endif

private:
	std::atomic<Node_Linked*>* prefix; 
//	char* arena_pointer;			
	std::atomic<char>* arena_pointer;
	//char* arena_pointer;
	string str_key;
	string str_value;

	int height;

//#ifdef STRIDE_SKIPLIST_H
	Node_Linked* stride_next;
//#endif

public:
#ifdef CV_SKIPLIST_H
	pthread_cond_t cond;
	bool done;
#endif

#if 1
	// Constructor 
	Node_Linked(const string& key_, const string& value_,int height_) 
		: str_key(key_), str_value(value_), height(height_)
	{
		prefix = new std::atomic<Node_Linked*> [height];

		for(int i=0; i < height; i++)
			(&prefix[i])->store(nullptr,std::memory_order_relaxed);	
		Set_vqueue(nullptr);
	}
#endif
	Node_Linked(Arena& arena_, const string& key_, const string& value_,int height_) 

		: str_key(key_), str_value(value_), height(height_)
	{
	
#if 1
		arena_pointer=arena_.AllocateAligned(sizeof(std::atomic<Node_Linked*>)*height + sizeof(Node_Linked));
		prefix = reinterpret_cast<std::atomic<Node_Linked*>*>(arena_pointer);
		for(int i=0; i < height; i++){
			(&prefix[i])->store(nullptr,std::memory_order_relaxed);	
		}
	
#endif			
		Set_vqueue(nullptr);
	}

	~Node_Linked(){
		delete[] prefix;	
	}


} Node_Linked;


#endif
