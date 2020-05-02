#ifndef ITERATOR_LINKED_H
#define ITERATOR_LINKED_H

#include "SkipList_Linked.h"
#include "../Arena.h"
#include "Node_Linked.h"
class SkipList_Linked;


class Iterator_Linked{

public:
	
	void Put(string key, string value, Iterator_Linked& iterator){
		sl_->Insert(key, value, iterator);
	}

        void SetList(SkipList_Linked* sl){
            sl_ = sl; 
        }
        bool Valid(){
            return (node_ != nullptr); 
        }
        void Next(){
            if(Valid() != 0)
            node_ = node_->Next();
        }
        void Seek(string key){
            node_ = sl_ ->FindGreaterorEqual(key);
	}
	struct Node_Linked* Node_Linked(){
            return node_;
        }
    private:
        SkipList_Linked* sl_ = nullptr;
        struct Node_Linked* node_;

    public:
	Arena arena;
	int seq;
	SkipList_Linked::Splice* splice;
	bool test;

public:
        Iterator_Linked(){
            sl_ = nullptr;
            node_ = nullptr;
	    	splice = sl_->AllocateSplice();	
        }

        Iterator_Linked(SkipList_Linked* sl, int seq_){
            SetList(sl);
	    	node_ = nullptr;
	    	splice = sl_->AllocateSplice();
			seq=seq_;
        }

};


#endif
