#ifndef NODE_H
#define NODE_H

#include<string>
#include<vector>
#include<queue>
#include "SkipList.h"

using namespace std;


//#ifdef JELLYFISH_SKIPLIST_H
typedef struct VNode {

    bool CASNext(VNode* expected, VNode* x) {
   // assert(n >= 0);
  
   // return (&next_[0] - n)->compare_exchange_strong(expected, x);
     
	return (&next)->compare_exchange_strong(expected, x);
  }
    string value; 
    string timestamp; 
    atomic<VNode*> next; 
}VNode;

//#endif

typedef struct Node {
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

  const char* Key() const { return reinterpret_cast<const char*>(&next_[1]); }

  // Accessors/mutators for links.  Wrapped in methods so we can add
  // the appropriate barriers as necessary, and perform the necessary
  // addressing trickery for storing links below the Node in memory.
  Node* Next(int n) {
   // assert(n >= 0);
    // Use an 'acquire load' so that we observe a fully initialized
    // version of the returned Node.
    return ((&next_[0] - n)->load(std::memory_order_acquire));
  }

  void SetNext(int n, Node* x) {
    //assert(n >= 0);
    // Use a 'release store' so that anybody who reads through this
    // pointer observes a fully initialized version of the inserted node.
    (&next_[0] - n)->store(x, std::memory_order_release);
  }

  bool CASNext(int n, Node* expected, Node* x) {
   // assert(n >= 0);
    return (&next_[0] - n)->compare_exchange_strong(expected, x);
  }

  // No-barrier variants that can be safely used in a few locations.
  Node* NoBarrier_Next(int n) {
   // assert(n >= 0);
    return (&next_[0] - n)->load(std::memory_order_relaxed);
  }

  void NoBarrier_SetNext(int n, Node* x) {
    //assert(n >= 0);
    (&next_[0] - n)->store(x, std::memory_order_relaxed);
  }

  // Insert node after prev on specific level.
  void InsertAfter(Node* prev, int level) {
    // NoBarrier_SetNext() suffices since we will add a barrier when
    // we publish a pointer to "this" in prev.
    NoBarrier_SetNext(level, prev->NoBarrier_Next(level));
    prev->SetNext(level, this);
  }

  void Set_key(string key){
    //assert(str_key != key);
      str_key = key;
      
  }
  
  string Get_key(){
      return str_key;
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
  bool CAS_vqueue(VNode* n){
//	if(__sync_bool_compare_and_swap(vqueue, nullptr, n)){
//		return 1;
//	}
//	else
		return 0;
  }

  void Set_vqueue(VNode* n){
	vqueue = n;
	vqueue_num++;
  }

  VNode* Get_vqueue(){
	return vqueue;
  }


  void Set_vqueue_num(int num){
	vqueue_num = num;
  }

  int Get_vqueue_num(){
	return vqueue_num;
  }
 #endif

private:
  // next_[0] is the lowest level link (level 0).  Higher levels are
  // stored _earlier_, so level 1 is at next_[-1].
  
  std::atomic<Node*> next_[1];
  string str_key ;
  string str_value ;
  int height = 1;
  #ifdef JELLYFISH_SKIPLIST_H
   VNode* vqueue;
   int vqueue_num;
  #endif
  
  //std::atomic<char*> value[1]; 
}Node;

///////////////////////////////////////////////
/////////////////////////////////////////////
//////////////////////////////////////////


#endif
