#include "ConcurrentSkipList.h"

ConcurrentSkipList::ConcurrentSkipList(int32_t max_height, int node_count)
    :SkipList(static_cast<uint16_t>(max_height), AllcocateNode("!","!",max_height),1,AllocateSplice()){
    srand((unsigned)time(NULL));

    for(int i=0; i<kMaxHeight_; i++){
	head_->SetNext(i, nullptr);
    }
}




bool ConcurrentSkipList::Insert(Node *nnode){


  int height = nnode->UnstashHeight();
  cout<<"nnode->str_key = "nnode->Get_key()<<endl;
  cout<<"nnode->str_value = "<<nnode->Get_value()<<endl;
  cout<<"height = "<<height<<endl;
  cout<<"max_height_ = "<<max_height_<<endl;
  int max_height = max_height_.load(std::memory_order_relaxed);

   while(height > max_height){
     if(max_height_.comapre_exchange_weak(max_height, height)){
	max_height = height;
	break;
     }
   }
   
   if(seq_splice->height_ < max_height){
      seq_splice->prev_[max_height] = head_;
      seq_splice->next_[max_height] = nullptr;
      seq_splice->height_ =max_height;
   }

}
