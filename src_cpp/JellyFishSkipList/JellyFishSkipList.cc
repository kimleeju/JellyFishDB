#include "JellyFishSkipList.h"

int JellyFishSkipList::Put(string key, string value, Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Put(key, value, iterator);
    return 0;
}


string JellyFishSkipList::Get(string key, Iterator iterator){
    t_global_committed.get_and_inc();
	iterator.Seek(key);
	Node* temp = iterator.Node();
	  if(temp -> Get_vqueue_num() != 0){
		return temp -> Get_vqueue() -> value; 
    }
    else	
		return temp->Get_value();
}




void JellyFishSkipList::RangeQuery(string start_key, int count, Iterator iterator){
    t_global_committed.get_and_inc();
//    cout<<"-----------------------------"<<endl;
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
      for(int i=count; i > 0; --i) {
		if(temp_->Get_vqueue_num() != 0){}
	//		cout<<"str_key, str_value = "<<temp_->Get_key()<<", "<<temp_->Get_vqueue()->value<<endl;
		else
		{}
      //  	cout<<"str_key, str_value = "<<temp_->Get_key()<<", "<<temp_->Get_value()<<endl;
		if(temp_->Next(0)==nullptr)
			return;
       	temp_=temp_->Next(0);
    } 
	return;
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
	int cmp = (next == nullptr || next == last_bigger) ? 1 : Comparator(key,next->Get_key());
	
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
//	cout<<"kMaxHeight = "<<kMaxHeight_<<endl;
    while(true){

        Node* next = x->Next(level);
      //  int cmp = (next == nullptr || next == last_bigger) ? 1 : Comparator(next->Get_key(),key);
		int cmp = (next == nullptr || next == last_bigger) ? 1 : KeyIsAfterNode(key,next);
	//cout<<"cnt = "<<++cnt	<<endl;
#if 0	
if(next!=nullptr && level != 0){
	cout<<"next->key = "<<next->Get_key()<<endl;
	cout<<"key = "<<key<<endl;
	cout<<"level = "<<level<<endl;
}
#endif	
		if(cmp==0){
			return next;
		}
        else if(cmp > 0 &&level ==0){
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
    /*size_t array_size = sizeof(Node*) * (kMaxHeight_ + 1) + sizeof(Node) ;
    char* raw = new char[sizeof(Splice) + array_size*2];
    Splice* splice = reinterpret_cast<Splice*>(raw);
    splice->height_ = 0;
    splice->prev_ = reinterpret_cast<Node**>(raw + sizeof(Splice) );
    splice->next_ = reinterpret_cast<Node**>(raw + sizeof(Splice) + array_size);*/
	
	Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node *[MAX_LEVEL];
	splice->next_ = new Node *[MAX_LEVEL];
    return splice;
}

int  JellyFishSkipList::RecomputeSpliceLevels(string key, int level, Splice* splice){
    Node* before = head_;
    for(int i = max_height_ -1  ;i>=0; --i){
        FindSpliceForLevel(key, level, i, &splice->prev_[i], &splice->next_[i],before);
		if(splice->next_[i]!=nullptr && (Comparator(key,splice->next_[i]->Get_key())==0)){
			return i;	
		}
	}
	return 0;
}

void JellyFishSkipList::FindSpliceForLevel(string key, int level, int cur_level,  Node** sp_prev, Node** sp_next, Node* before){
    Node* after = before ->Next(cur_level);
   while(true){
	
     if(!KeyIsAfterNode(key, after)){
        if(level >= cur_level){
		    *sp_prev = before;
            *sp_next = after;
		}
        return;
        }
        before = after;
        if(after != nullptr){
            after = after->Next(cur_level);
		}
    }
}

int JellyFishSkipList::Comparator(string key1, string key2){
#if 0	
	if(key1.length() < key2.length())
		return  -1;
	else if ( key1.length() > key2.length())
		return 1;
#endif
  return key1.compare(key2);
}


bool JellyFishSkipList::KeyIsAfterNode(string key, Node* n){
#if 0
	if(n == nullptr || key.length() < n->Get_key().length())
		return  0;
	else if ( key.length() > n->Get_key().length())
		return 1;
#endif
  return (n != nullptr) && (key.compare(n->Get_key()) > 0);
//	return (n != nullptr) && ((key > n->Get_key())?1:0);
}

VNode* JellyFishSkipList::AllocateVNode(string value){
   VNode* x = new VNode;
   x->value = value;
   x->NoBarrier_SetNext(nullptr);
   //x->timestamp = n->Get_vqueue_num();
   //x->next = n->Get_vqueue();
   return x; 
}


Node* JellyFishSkipList::AllocateNode(string key, string value, int height){
   Node* x = new Node(key, value, height);
   return x;
}

int JellyFishSkipList::RandomHeight(){
   int height, balancing, pivot;
   balancing = 2 ;
   height = 1;
   pivot = 1000/balancing;
   while(height < kMaxHeight_ && height < pivot && (rand()%1000)<pivot){
    height++;
   }
   return height;
}

JellyFishSkipList::JellyFishSkipList()
    :SkipList(static_cast<uint16_t>(MAX_LEVEL), AllocateNode("!","!",MAX_LEVEL),1,AllocateSplice()){
    srand((unsigned)time(NULL));
}

bool JellyFishSkipList::Insert(string key, string value, Iterator iterator){
  //int height = 4;
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
	
	int splice_index;
    if(height > 0) {
		splice_index = RecomputeSpliceLevels(key, height,iterator.splice);
    }

	iterator.test = 0;
while(!iterator.test){
#if 0
	if(iterator.splice->next_[splice_index]!=nullptr)
		cout<<"iterator.splice->next_[splice_index]->Get_key = "<<iterator.splice->next_[splice_index]->Get_key()<<endl;
	cout<<"key = "<<key<<endl; 
#endif
    if(iterator.splice->next_[splice_index]!=nullptr && iterator.splice->next_[splice_index]->Get_key() == key){
		VNode* nnode = AllocateVNode(value);
	     while(true){
			if(iterator.splice->next_[splice_index]->Get_vqueue_num()!=0)	
				nnode->NoBarrier_SetNext(iterator.splice->next_[splice_index]->Get_vqueue()->NoBarrier_Next());
		 	if(iterator.splice->next_[splice_index]->Get_vqueue()->CASNext(nnode->NoBarrier_Next(),nnode)){
				iterator.splice->next_[splice_index]->Set_vqueue_num();
			//iterator.splice->next_[0]->Set_vqueue(nnode);	
		  		iterator.test = 1;
				break;
	      }
		}
    }
    
     else{
		for(int i = 0; i < height ; ++i){
			Node* nnode = AllocateNode(key, value, height);
			while(true){
				nnode -> NoBarrier_SetNext(i, iterator.splice->next_[i]);
				nnode->Set_vqueue(AllocateVNode(value));
				if(iterator.splice->prev_[i]->CASNext(i, iterator.splice->next_[i], nnode)){
		  			//success
		  			iterator.test = 1;
		  			break;
	   			}
#if 1			
				Node* before = iterator.splice->prev_[i];
	 			FindSpliceForLevel(key, height, i, &iterator.splice->prev_[i], &iterator.splice->next_[i], before);
#endif
//				splice_index = RecomputeSpliceLevels(key, i, iterator.splice);
				++cnt;
				delete nnode;
			//	i =height-1;
				break;   
			}
		}
     }
  }

  return true;
     
}
