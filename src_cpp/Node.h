#ifndef NODE_H
#define NODE_H

#include<string>
#include<vector>
#include<queue>
#include "SkipList.h"
#include "Util.h"
using namespace std;



// VNode 
typedef struct VNode {
	VNode* NoBarrier_Next() {
		return (&next)->load(std::memory_order_relaxed);
	}

	void NoBarrier_SetNext(VNode* x) {
		(&next)->store(x,std::memory_order_relaxed);	
	}
	
	bool CASNext(VNode* expected, VNode* x) {
		return (&next)->compare_exchange_strong(expected, x);
	}

	string value; 
	string timestamp; 
	atomic<VNode*> next; 

}VNode;


// Node 
typedef struct Node {
public:

	Node* Next(int n) {
		return (&prefix[n])->load(std::memory_order_acquire);
	}

	void SetNext(int n, Node* x) {
		(&prefix[n])->store(x, std::memory_order_release);
	}

	bool CASNext(int n, Node* expected, Node* x) {
		return (&prefix[n])->compare_exchange_strong(expected, x);	
	}


	Node* NoBarrier_Next(int n) {
		return (&prefix[n])->load(std::memory_order_relaxed);
	}
				
	void NoBarrier_SetNext(int n, Node* x) {
		(&prefix[n])->store(x,std::memory_order_relaxed);	
	}

	void InsertAfter(Node* prev, int level) {
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
	atomic<VNode*> vqueue;
//	atomic<int> vqueue_num;


	bool CAS_vqueue(VNode* expected, VNode* n){
		return (&vqueue)->compare_exchange_strong(expected,n);
	}

	void Set_vqueue(VNode* n){
		(&vqueue)->store(n, std::memory_order_relaxed);
	}

	VNode* Get_vqueue(){
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
	void Set_stride_next(Node* next){
		stride_next = next;
	}

	Node* Get_stride_next(){
		return stride_next;
	}
//#endif

private:
	std::atomic<Node*> *prefix; 
				
	string str_key;
	string str_value;

	int height;

//#ifdef STRIDE_SKIPLIST_H
	Node* stride_next;
//#endif

public:
#ifdef CV_SKIPLIST_H
	pthread_cond_t cond;
	bool done;
#endif


	// Constructor 
	Node(const string& key_, const string& value_,int height_) 
		: str_key(key_), str_value(value_), height(height_)
	{
		prefix = new std::atomic<Node*> [height];

		for(int i=0; i < height; i++)
			(&prefix[i])->store(nullptr,std::memory_order_relaxed);	

//#ifdef JELLYFISH_SKIPLIST_H
		Set_vqueue(nullptr);
	//	vqueue_num = 0;
//#endif	
//#ifdef STRIDE_SKIPLIST_H
		//stride_next =nullptr;
//#endif

	}

	~Node(){
		delete[] prefix;	
	}


} Node;


#endif
