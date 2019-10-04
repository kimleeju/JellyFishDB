#include  "StrideSkipList.h"

int StrideSkipList::Put(string key, string value, Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Put(key,value, iterator);
    return 0;
}


string StrideSkipList::Get(string key, Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Seek(key);
    string get_value = iterator.Node()->Get_value();
    return get_value;
}



void StrideSkipList::RangeQuery(string start_key, int count, Iterator iterator){
    t_global_committed.get_and_inc();
//    cout<<"-----------------------------"<<endl;
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
    int i = count;
	while(i>0){  
		if(temp_->Get_stride_next()==nullptr){
			if(temp_->Next(0)==nullptr);
				return;
			temp_=temp_->Next(0);
			--i;
		}
		else{
//			cout<<"aaaaaa"<<endl;
       		temp_=temp_->Get_stride_next();
			--i;
    	}
	} 	
}

Node* StrideSkipList::FindLast(){
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

Node* StrideSkipList::FindLessThan(string key, Node** prev){
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

Node* StrideSkipList::FindGreaterorEqual(string key){
    Node* x = head_;
    int level = kMaxHeight_ -1;
    Node *last_bigger = nullptr;
    while(true){
        Node* next = x->Next(level);
        int cmp = (next == nullptr || next == last_bigger) ? 1 :Comparator(next->Get_key(),key);
       
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
 


StrideSkipList::Splice* StrideSkipList::AllocateSplice(){
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

int StrideSkipList::RecomputeSpliceLevels(string key, int level, int low,Splice* splice){
    for(int i = MAX_LEVEL-1  ;i>=low; --i){	
		if(i==MAX_LEVEL-1)
        	FindSpliceForLevel(key, level,  i, &splice->prev_[i], &splice->next_[i],head_);
		else
			FindSpliceForLevel(key, level,  i, &splice->prev_[i], &splice->next_[i],splice->prev_[i+1]);
	 }
	return 0;
}


void StrideSkipList::FindSpliceForLevel(string key, int level,  int cur_level, Node** sp_prev, Node** sp_next, Node* before){
	 Node* after = before ->Next(cur_level);
   while(true){
	
     if(!KeyIsAfterNode(key, after)){
            *sp_prev = before;
            *sp_next = after;
        return;
     }
        before = after;
            after = after->Next(cur_level);
    }
}

int StrideSkipList::Comparator(string key1, string key2){
#if 0	
	if(key1.length() < key2.length())
		return  -1;
	else if ( key1.length() > key2.length())
		return 1;
  return key1.compare(key2);
#endif
	return key1.compare(key2);
}

bool StrideSkipList::KeyIsAfterNode(string key, Node* n){
#if 0
	if(n == nullptr || key.length() < n->Get_key().length())
		return  0;
	else if ( key.length() > n->Get_key().length())
		return 1;
#endif
#if 0
	if(n == nullptr)
		return 0;
	else if(key.length() < n->Get_key().length())
		return 0;
	else if(key.length() > n->Get_key().length())
		return 1;
#endif
	return (n != nullptr) && (key.compare(n->Get_key()) > 0);
}


Node* StrideSkipList::AllocateNode(string key, string value, int height){
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

int StrideSkipList::RandomHeight(){
   int height, balancing, pivot;
   balancing =2 ;
   height = 1;
   pivot = 1000/balancing;
   while(height < kMaxHeight_ && height < pivot && (rand()%1000)<pivot){
    height++;
   }
   return height;
}

StrideSkipList::StrideSkipList()
    :SkipList(static_cast<uint16_t>(MAX_LEVEL), AllocateNode("!","!",MAX_LEVEL),1,AllocateSplice()){
    srand((unsigned)time(NULL));

    /*for(int i=0; i<kMaxHeight_; i++){
	head_->SetNext(i, nullptr);
    }*/
}




bool StrideSkipList::Insert(string key, string value, Iterator iterator){
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
	Node* nnode = AllocateNode(key, value, height);

	if(nnode->Get_height() > 0) {
		RecomputeSpliceLevels(nnode->Get_key(), nnode->Get_height(), 0,iterator.splice);
    }
    
	for(int i=0; i<height ; i++){
		while(true){
			nnode->NoBarrier_SetNext(i, iterator.splice->next_[i]);
			if(iterator.splice->prev_[i]->CASNext(i,iterator.splice->next_[i],nnode)){
				//success
				break;
			}
			
			RecomputeSpliceLevels(nnode->Get_key(), nnode->Get_height(), i,iterator.splice);
		}
	}
	
#if 0	
	while(iterator.splice->next_[0] != nullptr && iterator.splice->next_[0]->Get_key() == key){
		iterator.splice->next_[0] = iterator.splice->next_[0]->Next(0);
	i}
#endif
	if(iterator.splice->next_[0] != nullptr && iterator.splice->next_[0]->Get_key() == key)
	{	
		nnode->Set_stride_next(iterator.splice->next_[0]);
	}
   return true;
     
}
