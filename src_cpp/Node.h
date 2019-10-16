#ifndef NODE_H
#define NODE_H

#include<string>
#include<vector>
#include<queue>
#include "SkipList.h"
#include "Util.h"
using namespace std;


//#ifdef JELLYFISH_SKIPLIST_H
typedef struct VNode {
     VNode* NoBarrier_Next() {
	return next;
  //	return (&next)->load(std::memory_order_relaxed);
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

//#endif

typedef struct Node {
public:
  // Stores the height of the node in the memory location normally used for
  // next_[0].  This is used for passing data from AllocateKey to Insert.
  void StashHeight(const int height_) {
 //   assert(sizeof(int) <= sizeof(next_[0]));
    height = height_;
    //memcpy(static_cast<void*>(&next_[0]), &height_, sizeof(int));
  }

  // Retrieves the value passed to StashHeight.  Undefined after a call
  // to SetNext or NoBarrier_SetNext.
  int UnstashHeight() const {
    return height;
    //int rv;
    //memcpy(&rv, &next_[0], sizeof(int));
    //return rv;
  }


  // Accessors/mutators for links.  Wrapped in methods so we can add
  // the appropriate barriers as necessary, and perform the necessary
  // addressing trickery for storing links below the Node in memory.
  Node* Next(int n) {
   // assert(n >= 0);
    // Use an 'acquire load' so that we observe a fully initialized
    // version of the returned Node.
   //// return ((&next_[0] - n)->load(std::memory_order_acquire));
// return prefix[n].load(std::memory_order_acquire);
	//return prefix[n];
	return (&prefix[n])->load(std::memory_order_acquire);
  }

  void SetNext(int n, Node* x) {
    //assert(n >= 0);
    // Use a 'release store' so that anybody who reads through this
    // pointer observes a fully initialized version of the inserted node.
//	prefix[n]->store(x, std::memory_order_realse);	
//	prefix[n] = x;
 ///////   //(&next_[0] - n)->store(x, std::memory_order_release);
  	(&prefix[n])->store(x, std::memory_order_release);
  }

  bool CASNext(int n, Node* expected, Node* x) {
   // assert(n >= 0);
	return (&prefix[n])->compare_exchange_strong(expected, x);	
  //  return (&next_[0] - n)->compare_exchange_strong(expected, x);
  }

  // No-barrier variants that can be safely used in a few locations.
  Node* NoBarrier_Next(int n) {
   // assert(n >= 0);
  return (&prefix[n])->load(std::memory_order_relaxed);

 //return (&next_[0] - n)->load(std::memory_order_relaxed);
  }

  void NoBarrier_SetNext(int n, Node* x) {
    //assert(n >= 0);
	//prefix[n] = x;
	(&prefix[n])->store(x,std::memory_order_relaxed);	
  //  (&next_[0] - n)->store(x, std::memory_order_relaxed);
  }

  // Insert node after prev on specific level.
  void InsertAfter(Node* prev, int level) {
    // NoBarrier_SetNext() suffices since we will add a barrier when
    // we publish a pointer to "this" in prev.
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
    //assert(str_value != value);
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

 #ifdef JELLYFISH_SKIPLIST_H
  bool CAS_vqueue(VNode* expected, VNode* n){
	//return vqueue.compare_exchange_weak(expected, n);
	//return __sync_bool_compare_and_swap(vqueue, expected, n);
	vqueue = n;
	return true;
  }

  void Set_vqueue(VNode* n){
	vqueue = n;
	//(&vqueue)->store(n, std::memory_order_relaxed);
	vqueue_num++;
  }

  VNode* Get_vqueue(){
	return vqueue;
  }


  void Set_vqueue_num(){
	vqueue_num++;
  }

  int Get_vqueue_num(){
	return vqueue_num;
  }
 #endif

 #ifdef STRIDE_SKIPLIST_H
	void Set_stride_next(Node* next){
		stride_next = next;
	}
	
	Node* Get_stride_next(){
		return stride_next;
	}
 #endif


	// Constructor 
	Node(const string& key_, const string& value_,int height_)
		: str_key(key_), str_value(value_), height(height_){

			prefix = new std::atomic<Node*> [height];

			for(int i=0; i < height; i++){
					(&prefix[i])->store(nullptr,std::memory_order_relaxed);	
			}
#ifdef JELLYFISH_SKIPLIST_H
			//	 vqueue=nullptr;
			vqueue_num = 0;
#endif	
#ifdef STRIDE_SKIPLIST_H
			stride_next =nullptr;
#endif

	}

 ~Node(){
	delete[] prefix;	
}


private:
	std::atomic<Node*> *prefix; 
//  char str_key[30];
	string str_key;
	string str_value;

	int height;
  #ifdef JELLYFISH_SKIPLIST_H
   VNode* vqueue;
   int vqueue_num;
  #endif
  
  #ifdef STRIDE_SKIPLIST_H
   Node* stride_next;
  #endif


public:
  #ifdef CV_SKIPLIST_H
	pthread_cond_t cond;
	//Mutex mu;
    bool done;
  #endif


}Node;

///////////////////////////////////////////////
/////////////////////////////////////////////
//////////////////////////////////////////


#endif
