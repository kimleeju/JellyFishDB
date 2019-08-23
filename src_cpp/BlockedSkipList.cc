#include "BlockedSkipList.h"

int BlockedSkipList::put(string key, string value){
    t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
    Insert(AllocateNode(key, value, RandomHeight()));
    t_global_committed.mlock.unlock();
    return 0;
}

string BlockedSkipList::get(string key){
    t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
    string get_value = get_impl(key)->Get_value();
    t_global_committed.mlock.unlock();
    return get_value;
}
    

Node* BlockedSkipList::get_impl(string key){
    Iterator iterator(this);
    iterator.Seek(key);
    return iterator.Node();
}


void BlockedSkipList::RangeQuery(string start_key, int count){
    t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
    cout<<"-----------------------------"<<endl;
    Iterator iterator(this);
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
      for(int i=count; i > 0; --i) {
        cout<<"str_key, str_value = "<<temp_->Get_key()<<", "<<temp_->Get_value()<<endl;
	if(temp_->Next(0)!=nullptr)
       	    temp_=temp_->Next(0);
    } 
    t_global_committed.mlock.unlock();
}



BlockedSkipList::Splice* BlockedSkipList::AllocateSplice(){
    size_t array_size = sizeof(Node*) * (kMaxHeight_ + 1) + sizeof(Node) ;
    char* raw = new char[sizeof(Splice) + array_size*2];
    Splice* splice = reinterpret_cast<Splice*>(raw);
    splice->height_ = 0;
    splice->prev_ = reinterpret_cast<Node**>(raw + sizeof(Splice) );
    splice->next_ = reinterpret_cast<Node**>(raw + sizeof(Splice) + array_size);
    return splice;
}


Node* BlockedSkipList::FindLast(){
    Node* x = head_;
    int level = kMaxHeight_ - 1;
    while(true){
        Node* next = x->Next(level);
        if(next == nullptr){
            if(level == 0){
                return x;
            }
            else{
                level--;
            }
        }
        else{
            x = next;
        }
    }
}

Node* BlockedSkipList::FindLessThan(string key, Node** prev){
    int level = kMaxHeight_ -1 ;
    Node* x = head_;
    Node* last_not_after = nullptr;
    while(true){
        Node* next = x->Next(level);
        if(next != last_not_after && KeyIsAfterNode(key,next)){
            x = next;
        }
        else{
            if(prev != nullptr){
                prev[level] = x;
            } 
            if(level==0){
                return x;
            }
            else{
                last_not_after = next;
                level--;
            }
        }
    }
}

Node* BlockedSkipList::FindGreaterorEqual(string key){
    Node* x = head_;
    int level = kMaxHeight_ -1;
    Node *last_bigger = nullptr;
    while(true){
        Node* next = x->Next(level);
        int cmp = (next == nullptr || next == last_bigger) ? 1 : next->Get_key().compare(key);

        if(cmp >= 0 &&level ==0){
            return next;
        }
        else if (cmp < 0){
//	 if(next->Next(level) !=nullptr)	
            x= next;
//	 else
//	    return nullptr;
        }
        else{
            last_bigger = next;
            level --;
        }
    }
}
    


void BlockedSkipList::RecomputeSpliceLevels(Node* node, int level, Splice* splice){
    Node* before = head_;
    for(int i =level -1  ;i>=0; --i){
        FindSpliceForLevel(node, i, &seq_splice->prev_[i], &seq_splice->next_[i],before);
    }
}


void BlockedSkipList::FindSpliceForLevel(Node* node, int level, Node** sp_prev, Node** sp_next, Node* before){
    Node* after = before ->Next(level);
    while(true){
        if(!KeyIsAfterNode(node->Get_key(), after)){
            *sp_prev = before;
            *sp_next = after;
            return;
        }
        before = after;
        if(after != nullptr)
            after = after->Next(level);
    }
}

bool BlockedSkipList::KeyIsAfterNode(string key, Node* n){
     return (n != nullptr) && (key.compare(n->Get_key()) > 0);
} 



Node* BlockedSkipList::AllocateKey(){
	return AllocateNode(randomString(),randomString(),RandomHeight());
}

Node* BlockedSkipList::AllocateNode(string key, string value, int height){
   auto prefix = sizeof(Node*) * (height);
   char* raw = new char [prefix  + sizeof(Node*)+sizeof(Node)];
   Node* x = reinterpret_cast<Node*>(raw + prefix);

   for(int i=0;i<height;i ++){
       x->SetNext(i,nullptr);
       //assert(x->Next(i));
   }
   x->StashHeight(height);
   x->Set_key(key);
   x->Set_value(value);
   
   return x;
} 

int BlockedSkipList::RandomHeight(){
   int height, balancing, pivot;
   balancing =2 ;
   height = 1;
   pivot = 1000/balancing;
   while(height < kMaxHeight_ && height < pivot && (rand()%1000)<pivot){
    height++;
   }
   return height;
}



BlockedSkipList::BlockedSkipList(int32_t max_height,int node_count)
    :SkipList(static_cast<uint16_t>(max_height), AllocateNode("!","!",max_height),1,AllocateSplice()){
   srand((unsigned)time(NULL));
   
   for(int i=0; i<kMaxHeight_;i++){
        head_->SetNext(i, nullptr);
   }
}

 


bool BlockedSkipList::Insert(Node *nnode){


  int height = nnode->UnstashHeight();
  cout<<"nnode->str_key = "<<nnode->Get_key()<<endl;
  cout<<"nnode->str_value = "<<nnode->Get_value()<<endl;
  cout<<"height = "<<height<<endl;
  cout<<"max_height_ = "<<max_height_<<endl;
  int max_height = max_height_.load(std::memory_order_relaxed);

    if(height > max_height){
	max_height = height;   
    }
    

    if(seq_splice->height_ < max_height){
       seq_splice->prev_[max_height] = head_;
       seq_splice->next_[max_height] = nullptr;
       seq_splice->height_ = max_height; 
    }
    
    else{
        for(int i = 0; i< height ; i++){
           seq_splice->prev_[i] = head_;
           seq_splice->next_[i] = seq_splice->prev_[i]->Next(i);
        }
    }

     if(height > 0){
        RecomputeSpliceLevels(nnode, height);
    }


     for(int i=0;i<height;++i){  
        nnode->SetNext(i, seq_splice->next_[i]);
        seq_splice->prev_[i]->SetNext(i,nnode);

    }
    for(int i = 0; i< height ; i++){
        seq_splice->prev_[i] = head_;
        seq_splice->next_[i] = seq_splice->prev_[i]->Next(i);
    }
   

  return true;

}




