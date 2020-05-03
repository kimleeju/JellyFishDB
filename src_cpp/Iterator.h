#ifndef ITERATOR_H
#define ITERATOR_H

#include "SkipList.h"
#include "Node.h"
#include "Arena.h"
class SkipList;


class Iterator{

public:
	
	void Put(string key, string value, Iterator& iterator){
		sl_->Insert(key, value, iterator);
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
        void Seek(string key){
            node_ = sl_ ->FindGreaterorEqual(key);
		}
	struct Node* Node(){
            	if(Valid() == 0)
			return NULL;
		else
			return node_;
        }
    private:
        SkipList* sl_ = nullptr;
        struct Node* node_;

    public:
		Arena arena;
		int seq;
		SkipList::Splice* splice;
		bool test;

public:
        Iterator(){
            sl_ = nullptr;
            node_ = nullptr;
	    	splice = sl_->AllocateSplice();	
        }

        Iterator(SkipList* sl, int seq_){
            SetList(sl);
	    	node_ = nullptr;
	    	splice = sl_->AllocateSplice();
			seq=seq_;
        }

};


#endif
