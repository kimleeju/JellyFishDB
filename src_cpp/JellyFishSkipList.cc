#include "JellyFishSkipList.h"

int JellyFishSkipList::put(string key, string value){
    t_global_committed.get_and_inc();
    put_impl(key, value);
    return 0;
}

void JellyFishSkipList::put_impl(string key, string value){
    Iterator iterator(this);
    iterator.Put(key, value);
}	

string JellyFishSkipList::get(string key){
    t_global_committed.get_and_inc();
    Node* temp = get_impl(key);
    if(temp -> Get_vqueue_num() != 0){
	return temp -> Get_vqueue() -> value; 
    }
    else	
	 return temp->Get_value();
}

Node* JellyFishSkipList::get_impl(string key){
    Iterator iterator(this);
    iterator.Seek(key);
    return iterator.Node();
}


void JellyFishSkipList::RangeQuery(string start_key, int count){
    t_global_committed.get_and_inc();
    cout<<"-----------------------------"<<endl;
    Iterator iterator(this);
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
      for(int i=count; i > 0; --i) {
	if(temp_->Get_vqueue_num() != 0)
		cout<<"str_key, str_value = "<<temp_->Get_key()<<", "<<temp_->Get_vqueue()->value<<endl;
	else
        	cout<<"str_key, str_value = "<<temp_->Get_key()<<", "<<temp_->Get_value()<<endl;
	if(temp_->Next(0)!=nullptr)
       	    temp_=temp_->Next(0);
	else
		return;
    } 
}

Node* JellyFishSkipList::FindLast(){
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

Node* JellyFishSkipList::FindLessThan(string key, Node** prev){
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

Node* JellyFishSkipList::FindEqual(string key){ 
    Node* x= head_;
    int level = kMaxHeight_ -1;
    Node* last_bigger = nullptr;
    while(true){
	Node* next = x->Next(level);
	int cmp = (next == nullptr || next == last_bigger) ? 1 : next->Get_key().compare(key);
	
	if(cmp == 0) {
		return next;
	}
	else if(cmp > 0 && level == 0){
		return nullptr;
	}

	else if(cmp < 0){
	    x= next;
	}
	else{
	   last_bigger = next;
	    level--;
 	}
    }
}

Node* JellyFishSkipList::FindGreaterorEqual(string key){
    Node* x = head_;
    int level = kMaxHeight_ -1;
    Node *last_bigger = nullptr;
    while(true){
        Node* next = x->Next(level);
	cout<<"level = "<<level<<endl;
	cout<<"next = "<<next<<endl;
        int cmp = (next == nullptr || next == last_bigger) ? 1 : next->Get_key().compare(key);

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

JellyFishSkipList::Splice* JellyFishSkipList::AllocateSplice(){
    size_t array_size = sizeof(Node*) * (kMaxHeight_ + 1) + sizeof(Node) ;
    char* raw = new char[sizeof(Splice) + array_size*2];
    Splice* splice = reinterpret_cast<Splice*>(raw);
    splice->height_ = 0;
    splice->prev_ = reinterpret_cast<Node**>(raw + sizeof(Splice) );
    splice->next_ = reinterpret_cast<Node**>(raw + sizeof(Splice) + array_size);
    return splice;
}

void JellyFishSkipList::RecomputeSpliceLevels(string key, int level, Splice* splice){
    Node* before = head_;
    for(int i =level -1  ;i>=0; --i){
        FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i],before);
    }
}

void JellyFishSkipList::FindSpliceForLevel(string key, int level, Node** sp_prev, Node** sp_next, Node* before){
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


bool JellyFishSkipList::KeyIsAfterNode(string key, Node* n){
     return (n != nullptr) && (key.compare(n->Get_key()) > 0);
}

VNode* JellyFishSkipList::AllocateVNode(string value){
   VNode* x = new VNode;
   x->value = value;
   //x->timestamp = n->Get_vqueue_num();
   //x->next = n->Get_vqueue();
   return x; 
}


Node* JellyFishSkipList::AllocateNode(string key, string value, int height){
   auto prefix = sizeof(Node*) * (height);
   char* raw = new char [prefix  + sizeof(Node*)+sizeof(Node)];
   Node* x = reinterpret_cast<Node*>(raw + prefix);
   for(int i=0;i<height;i ++){
       x->SetNext(i,nullptr);
       //assert(x->Next(i))
   }
   x->Set_vqueue(nullptr);
   x->Set_vqueue_num (0);
   x->StashHeight(height);
   x->Set_key(key);
   x->Set_value(value);
   
   return x;
}

int JellyFishSkipList::RandomHeight(){
   int height, balancing, pivot;
   balancing =2 ;
   height = 1;
   pivot = 1000/balancing;
   while(height < kMaxHeight_ && height < pivot && (rand()%1000)<pivot){
    height++;
   }
   return height;
}

JellyFishSkipList::JellyFishSkipList(int32_t max_height, int node_count)
    :SkipList(static_cast<uint16_t>(max_height), AllocateNode("!","!",max_height),1,AllocateSplice()){
    srand((unsigned)time(NULL));

    for(int i=0; i<kMaxHeight_; i++){
	head_->SetNext(i, nullptr);
    }
}

bool JellyFishSkipList::Insert(string key, string value){
  
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
   	if(splice->next_[0]!=nullptr &&splice->next_[0]->Get_key() == key){
		VNode* nnode = AllocateVNode(value);	
		nnode->next = splice->prev_[0]->Get_vqueue();
	//	position_node->Get_vqueue()->next.compare_exchange_strong(nnode->next,nnode);

	//	position_node->Get_vqueue()->CASNext(nnode->next,nnode);   
	// 	if((__sync_val_compare_and_swap(position_node->Get_vqueue()->next, nnode->next, nnode))== nnode->next){}
		splice->next_[0]->Set_vqueue(nnode);	
		cout<<"nnode -> value = "<<nnode->value<<endl;
		cout<<"splice->next_[0] -> Get_vqueue_num() = "<<splice->next_[0]->Get_vqueue_num()<<endl;
	}

	else{
		for(int i = 0; i < height ; ++i){
			while(true){
				Node* nnode = AllocateNode(key, value, height);
	   			nnode -> NoBarrier_SetNext(i, splice->next_[i]);
	   			if(splice->prev_[i]->CASNext(i, splice->next_[i], nnode)){
					//success
					break;
	   			}
	
		 		Node* before = head_;
		   		FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], before);
	    		}
		}
		cout<<"nnode->str_key = "<<key<<endl;
  		cout<<"nnode->str_value = "<<value<<endl;
  		cout<<"height = "<<height<<endl;
  		cout<<"max_height_ = "<<max_height_<<endl;
  	}

   return true;
     
}
