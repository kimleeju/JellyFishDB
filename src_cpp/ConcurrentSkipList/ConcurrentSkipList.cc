#include  "ConcurrentSkipList.h"

int ConcurrentSkipList::Put(string key, string value, Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Put(key,value, iterator);
    return 0;
}


string ConcurrentSkipList::Get(string key, Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Seek(key);
    string get_value = iterator.Node()->Get_value();
    return get_value;
}



void ConcurrentSkipList::RangeQuery(string start_key, int count, Iterator iterator){
    t_global_committed.get_and_inc();
//    cout<<"-----------------------------"<<endl;
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
//		cout<<"cnt = "<<++cnt<<endl;
#if 0
		cout<<"--------------------------"<<endl;
		cout<<"key = "<<key<<endl;
		if(next!=nullptr)
		cout<<"next->Get_key() = "<<next->Get_key()<<endl;
		cout<<"level = "<<level<<endl;
#endif
        if(cmp >= 0 &&level ==0){
            return next;
        }
        else if (cmp < 0){
            x= next;
        }
        else{
            last_bigger = next;
            level --;
        }
    }
}
 


ConcurrentSkipList::Splice* ConcurrentSkipList::AllocateSplice(){
   /* size_t array_size = sizeof(Node*) * (kMaxHeight_ + 1) + sizeof(Node) ;
    char* raw = new char[sizeof(Splice) + array_size*2];
    Splice* splice = reinterpret_cast<Splice*>(raw);
    splice->height_ = 0;
    splice->prev_ = reinterpret_cast<Node**>(raw + sizeof(Splice) );
    splice->next_ = reinterpret_cast<Node**>(raw + sizeof(Splice) + array_size);
    return splice;*/
	Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node *[MAX_LEVEL];
	splice->next_ = new Node *[MAX_LEVEL];
   return splice;
}

int ConcurrentSkipList::RecomputeSpliceLevels(string key, int level, Splice* splice){
    Node* before = head_;
    for(int i =level -1  ;i>=0; --i){
        FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i],before);
    }
	return 0;
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
 	if(n == nullptr || key.length() < n->Get_key().length())
		return  0;
	else if ( key.length() > n->Get_key().length())
		return 1;
    return (n != nullptr) && (key.compare(n->Get_key()) > 0);
}


Node* ConcurrentSkipList::AllocateNode(string key, string value, int height){
   /*auto prefix = sizeof(Node*) * (height);
   char* raw = new char [prefix  + sizeof(Node*)+sizeof(Node)];
   Node* x = reinterpret_cast<Node*>(raw + prefix);

   for(int i=0;i<height;i ++){
       x->SetNext(i,nullptr);
       //assert(x->Next(i))
   }
	
   x->StashHeight(height);
   x->Set_key(key);
   x->Set_value(value);*/
   Node* x = new Node(key,value, height);
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

ConcurrentSkipList::ConcurrentSkipList()
    :SkipList(static_cast<uint16_t>(MAX_LEVEL), AllocateNode("!","!",MAX_LEVEL),1,AllocateSplice()){
    srand((unsigned)time(NULL));

    /*for(int i=0; i<kMaxHeight_; i++){
	head_->SetNext(i, nullptr);
    }*/
}




bool ConcurrentSkipList::Insert(string key, string value, Iterator iterator){
  //Node* nnode = AllocateNode(key, value, RandomHeight());
//  Splice* splice = AllocateSplice();
  int height = RandomHeight();
  int max_height = max_height_.load(std::memory_order_relaxed);
   while(height > max_height){
     if(max_height_.compare_exchange_weak(max_height, height)){
	max_height = height;
	break;
     }
  } 
	 
  if(iterator.splice->height_ < max_height){
      iterator.splice->prev_[max_height] = head_;
      iterator.splice->next_[max_height] = nullptr;
      iterator.splice->height_ =max_height;
   }
   else{
	for(int i = 0; i<height ; i++){
	   iterator.splice->prev_[i] = head_;
	   iterator.splice->next_[i] = iterator.splice->prev_[i] ->NoBarrier_Next(i);
	}
    }

    if(height > 0) {
	RecomputeSpliceLevels(key, height,iterator.splice);
    }
    Node* nnode = AllocateNode(key, value, height);
    
    for(int i = 0; i < height ; ++i){
	while(true){
	   nnode -> NoBarrier_SetNext(i, iterator.splice->next_[i]);
	   if(iterator.splice->prev_[i]->CASNext(i, iterator.splice->next_[i], nnode)){
		//success
		break;
	   }
	
	   Node* before = head_;
	   FindSpliceForLevel(key, i, &iterator.splice->prev_[i], &iterator.splice->next_[i], before);
		
	}
    }
  //cout<<"nnode->str_key = "<<nnode->Get_key()<<endl;
  //cout<<"nnode->str_value = "<<nnode->Get_value()<<endl;
  //cout<<"height = "<<height<<endl;
  //cout<<"max_height_ = "<<max_height_<<endl;
   return true;
     
}
