#include  "ConcurrentSkipList.h"

int ConcurrentSkipList::put(string key, string value){
    t_global_committed.get_and_inc();
    //Insert(AllocateNode(key, value, RandomHeight()));
    put_impl(key, value);
    return 0;
}

void ConcurrentSkipList::put_impl(string key, string value){
    Iterator iterator(this);
    iterator.Put(key, value);
}	

string ConcurrentSkipList::get(string key){
    t_global_committed.get_and_inc();
    string get_value = get_impl(key)->Get_value();
    return get_value;
}

Node* ConcurrentSkipList::get_impl(string key){
    Iterator iterator(this);
    iterator.Seek(key);
    return iterator.Node();
}


void ConcurrentSkipList::RangeQuery(string start_key, int count){
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
}

Node* ConcurrentSkipList::FindLast(){
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

Node* ConcurrentSkipList::FindLessThan(string key, Node** prev){
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

Node* ConcurrentSkipList::FindGreaterorEqual(string key){
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
 


ConcurrentSkipList::Splice* ConcurrentSkipList::AllocateSplice(){
    size_t array_size = sizeof(Node*) * (kMaxHeight_ + 1) + sizeof(Node) ;
    char* raw = new char[sizeof(Splice) + array_size*2];
    Splice* splice = reinterpret_cast<Splice*>(raw);
    splice->height_ = 0;
    splice->prev_ = reinterpret_cast<Node**>(raw + sizeof(Splice) );
    splice->next_ = reinterpret_cast<Node**>(raw + sizeof(Splice) + array_size);
    return splice;
}

void ConcurrentSkipList::RecomputeSpliceLevels(string key, int level, Splice* splice){
    Node* before = head_;
    for(int i =level -1  ;i>=0; --i){
        FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i],before);
    }
}


void ConcurrentSkipList::FindSpliceForLevel(string key, int level, Node** sp_prev, Node** sp_next, Node* before){
    Node* after = before ->Next(level);
   while(true){
	
     if(!KeyIsAfterNode(key, after)){
            *sp_prev = before;
            *sp_next = after;
            return;
        }
        before = after;
        if(after != nullptr){
            after = after->Next(level);
	}
    }
}

bool ConcurrentSkipList::KeyIsAfterNode(string key, Node* n){
     return (n != nullptr) && (key.compare(n->Get_key()) > 0);
}


Node* ConcurrentSkipList::AllocateNode(string key, string value, int height){
   auto prefix = sizeof(Node*) * (height);
   char* raw = new char [prefix  + sizeof(Node*)+sizeof(Node)];
   Node* x = reinterpret_cast<Node*>(raw + prefix);

   for(int i=0;i<height;i ++){
       x->SetNext(i,nullptr);
       //assert(x->Next(i))
   }
	
   x->StashHeight(height);
   x->Set_key(key);
   x->Set_value(value);
   
   return x;
}

int ConcurrentSkipList::RandomHeight(){
   int height, balancing, pivot;
   balancing =2 ;
   height = 1;
   pivot = 1000/balancing;
   while(height < kMaxHeight_ && height < pivot && (rand()%1000)<pivot){
    height++;
   }
   return height;
}

ConcurrentSkipList::ConcurrentSkipList(int32_t max_height, int node_count)
    :SkipList(static_cast<uint16_t>(max_height), AllocateNode("!","!",max_height),1,AllocateSplice()){
    srand((unsigned)time(NULL));

    for(int i=0; i<kMaxHeight_; i++){
	head_->SetNext(i, nullptr);
    }
}




bool ConcurrentSkipList::Insert(string key, string value){
  //Node* nnode = AllocateNode(key, value, RandomHeight());
  Splice* splice = AllocateSplice();
  int height = RandomHeight();
  int max_height = max_height_.load(std::memory_order_relaxed);
   while(height > max_height){
     if(max_height_.compare_exchange_weak(max_height, height)){
	max_height = height;
	break;
     }
  } 
	 
  if(splice->height_ < max_height){
      splice->prev_[max_height] = head_;
      splice->next_[max_height] = nullptr;
      splice->height_ =max_height;
   }
   else{
	for(int i = 0; i<height ; i++){
	   splice->prev_[i] = head_;
	   splice->next_[i] = splice->prev_[i] ->NoBarrier_Next(i);
	}
    }

    if(height > 0) {
	RecomputeSpliceLevels(key, height,splice);
    }
    Node* nnode = AllocateNode(key, value, height);
    
    for(int i = 0; i < height ; ++i){
	while(true){
	   nnode -> NoBarrier_SetNext(i, splice->next_[i]);
	   if(splice->prev_[i]->CASNext(i, splice->next_[i], nnode)){
		//success
		break;
	   }
	
	   Node* before = head_;
	   FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], before);
		
	}
    }
	
  cout<<"nnode->str_key = "<<nnode->Get_key()<<endl;
  cout<<"nnode->str_value = "<<nnode->Get_value()<<endl;
  cout<<"height = "<<height<<endl;
  cout<<"max_height_ = "<<max_height_<<endl;
   return true;
     
}
