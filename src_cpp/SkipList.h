
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
/*class Iterator{
    public:
        Iterator(){
            sl_ = nullptr;
            node_ = nullptr;
        }

        Iterator(SkipList* sl){
            SetList(sl);
        }
	
	void Put(string key, string value){
	    sl_->Insert(key, value);
	}

        void SetList(SkipList* sl){
            sl_ = sl; 
        }
        bool Valid(){
            return (node_ != nullptr); 
        }
        void Next(){
            if(Valid() != 0)
            node_ = node_->Next(0);
        }
        void Prev(){
            Node** prev_ = nullptr;
            node_ = sl_->FindLessThan(node_->Get_key(),prev_);
        }
        void Seek(string key){
            node_ = sl_ ->FindGreaterorEqual(key);
        }
        void SeekForPrev(string key){
            Seek(key);
            if(!Valid()){
                SeekLast();
            }
            while (Valid() && key.compare(node_->Get_key())) 
                Prev();
        }
        void SeekFirst(){
            node_ = sl_->head_->Next(0);
        }
        void SeekLast(){
            node_= sl_->FindLast();
            if(node_ == sl_->head_){
                node_ = nullptr;
            }
        }
        Node* Node(){
            return node_;
        }

    private:
        SkipList* sl_ = nullptr;
        struct Node* node_;
};
*/

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

protected:
    uint16_t kMaxHeight_;
    atomic<int>max_height_; 
    Splice* seq_splice;
	int cnt =0 ;
    TimeStamp t_global_committed;
    
public:    
    string randomString();
    virtual int put(string key, string value, Iterator iterator ) = 0;
    virtual void put_impl(string key, string value) = 0;
    virtual string get(string key, Iterator iterator) = 0;
    virtual Node* get_impl(string key) = 0;
    virtual void RangeQuery(string start_key, int count, Iterator iterator) = 0;
    virtual Splice* AllocateSplice() =0;
    virtual Node* FindLast() =0;
    virtual Node* FindLessThan(string key, Node** prev)=0;
    virtual Node* FindGreaterorEqual(string key)=0;
    virtual void RecomputeSpliceLevels(string key, int level, Splice* splice = 0)=0;
    virtual void FindSpliceForLevel(string key, int level, Node** sp_prev, Node** sp_next, Node* before)=0;
    virtual bool KeyIsAfterNode(string key, Node* n)=0;
    virtual Node* AllocateNode(string key, string value, int height)=0;
    virtual int RandomHeight()=0;
    virtual bool Insert(string key, string value ,Iterator iterator)=0;
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
