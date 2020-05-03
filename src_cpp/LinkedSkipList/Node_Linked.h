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
	VNode_Linked(const string & value_):value(value_){}	

	string value; 
	string timestamp; 
	atomic<VNode_Linked*> next; 

}VNode_Linked;


// Node_Linked 
typedef struct Node_Linked{
public:

	Node_Linked* Next() {
		return (&prefix)->load(std::memory_order_acquire);
	}

	void SetNext(Node_Linked* x) {
		(&prefix)->store(x, std::memory_order_release);	
	}

	bool CASNext( Node_Linked* expected, Node_Linked* x) {
		return (&prefix)->compare_exchange_strong(expected, x);	
	}


	Node_Linked* NoBarrier_Next() {
		return (&prefix)->load(std::memory_order_relaxed);
	}
				
	Node_Linked* Get_Down(){
		return (&Down)->load(std::memory_order_acquire);
	}

	void SetDown(Node_Linked* x) {
		(&Down)->store(x,std::memory_order_relaxed);	
	}
	
	void NoBarrier_SetNext(Node_Linked* x) {
		(&prefix)->store(x,std::memory_order_relaxed);	
	}

	void InsertAfter(Node_Linked* prev) {
		NoBarrier_SetNext(prev->NoBarrier_Next());
		prev->SetNext(this);
	}

	void Set_key(const string& key){
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
	std::atomic<Node_Linked*> prefix; 
	std::atomic<Node_Linked*> Down;
//	char* arena_pointer;			
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
		: str_key(key_),  height(height_)
	{	
		if(value_ != "NULL")
			str_value = value_;
		(&prefix)->store(nullptr,std::memory_order_relaxed);	
		Set_vqueue(nullptr);
	}
#endif
	Node_Linked(Arena& arena_, const string& key_, const string& value_,int height_) 
		: str_key(key_),str_value(value_), height(height_)
	{
#if 1
		//std::atomic<char>* arena_pointer = arena_.AllocateAligned(sizeof(Node_Linked));	
		//prefix = reinterpret_cast<std::atomic<Node_Linked*>>(arena_pointer);
		(&prefix)->store(nullptr,std::memory_order_relaxed);	
#endif			
		Set_vqueue(nullptr);
	}

	~Node_Linked(){
		delete[] prefix;	
	}


} Node_Linked;


#endif
