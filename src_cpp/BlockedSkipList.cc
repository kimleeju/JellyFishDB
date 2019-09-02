#include "BlockedSkipList.h"

int BlockedSkipList::put(string key, string value){
    t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
   // Insert(AllocateNode(key, value, RandomHeight()));
    put_impl(key, value);
    t_global_committed.mlock.unlock();
    return 0;
}

void BlockedSkipList::put_impl(string key, string value){
    Iterator iterator(this);
    iterator.Put(key, value);
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
cout<<"******"<<endl;
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
    


void BlockedSkipList::RecomputeSpliceLevels(string key, int level, Splice* splice){
    Node* before = head_;
    for(int i =level -1  ;i>=0; --i){
        FindSpliceForLevel(key, i, &seq_splice->prev_[i], &seq_splice->next_[i],before);
    }
}


void BlockedSkipList::FindSpliceForLevel(string key, int level, Node** sp_prev, Node** sp_next, Node* before){
    Node* after = before ->Next(level);
    while(true){
        if(!KeyIsAfterNode(key, after)){
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




Node* BlockedSkipList::AllocateNode(string key, string value, int height){
   cout<<"height = "<<height <<endl; 
   //auto prefix = sizeof(atomic<Node*>) * (height-1);
   //cout<<"prefix = "<<prefix<<endl;
   //char* raw = new char [prefix +sizeof(Node)];
   //cout<<"raw = "<<raw<<endl;
   //printf("raw = %p\n",raw);
   
  //Node* x = reinterpret_cast<Node*>(raw + prefix);   
  //cout<<"x->Get_eky() = "<<x->Get_key()<<endl;
   Node* x = new Node(key,value,height);
   for(int i=0;i<height;i ++){
       x->SetNext(i,nullptr);
       //assert(x->Next(i));
   }
cout<<"sizeof(*x) = "<<sizeof(*x)<<endl;
//cout<<"str_key = "<<x->Get_key()<<endl;
//cout<<"UnstashHeight() = "<<x->UnstashHeight()<<endl;
//cout<<"str_key = "<<x->Get_key().capacity()<<endl;
//cout<<"str_key.capacity() = "<<x->str_key.capacity()<<endl;
//cout<<"sizeof(string) = "<<sizeof(string)<<endl;
//cout<<"sizeof(key) = " <<sizeof(key)<<endl;   
//cout<<"key.capacity() = "<<key.capacity()<<endl;
 
//   x->Set_key(key);
//   x->StashHeight(height);
//   x->Set_key(key);
//   x->Set_value(value);
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


BlockedSkipList::BlockedSkipList()
    :SkipList(static_cast<uint16_t>(MAX_LEVEL), AllocateNode("!","!",MAX_LEVEL),1,AllocateSplice()){
    srand((unsigned)time(NULL));
   for(int i=0; i<kMaxHeight_;i++){
        head_->SetNext(i, nullptr);
   }
}


bool BlockedSkipList::Insert(string key, string value){
 // Node* nnode = AllocateNode(key, value, RandomHeight());
  int height = RandomHeight();
  int max_height = max_height_.load(std::memory_order_relaxed);
    if(height > max_height){
	max_height_ = height;
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
        RecomputeSpliceLevels(key, height);
    }
	cout<<"sizeof(Node) = "<<sizeof(Node)<<endl;
     Node* nnode = AllocateNode(key, value, height);
    cout<<"sizeof(*nnode) ="<< sizeof(*nnode)<<endl;
     for(int i=0;i<height;++i){  
        nnode->SetNext(i, seq_splice->next_[i]);
        seq_splice->prev_[i]->SetNext(i,nnode);

    }

    for(int i = 0; i< height ; i++){
        seq_splice->prev_[i] = head_;
        seq_splice->next_[i] = seq_splice->prev_[i]->Next(i);
    }
   cout<<"nnode->str_key = "<<nnode->Get_key()<<endl;
  cout<<"nnode->str_value = "<<nnode->Get_value()<<endl;
  cout<<"height = "<<height<<endl;
  cout<<"max_height_ = "<<max_height_<<endl;
 cout<<"cnt = "<<++cnt<<endl; 
cout<<"-------------------------------"<<endl;
  return true;

}




