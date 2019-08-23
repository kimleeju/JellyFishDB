#ifndef ITERATOR_H
#define ITERATOR_H

#include "SkipList.h"
#include "Node.h"

class Iterator{
    public:
        Iterator(){
            sl_ = nullptr;
            node_ = nullptr;
        }

        Iterator(SkipList* sl){
            SetList(sl);
        }
	
	void Put(string key, string value){
	    sl_->Insert(sl_->AllocateNode(key, value, sl_->RandomHeight()));
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
            struct Node** prev_ = nullptr;
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
        struct Node* Node(){
            return node_;
        }

    private:
        SkipList* sl_ = nullptr;
        struct Node* node_;
};
#endif
