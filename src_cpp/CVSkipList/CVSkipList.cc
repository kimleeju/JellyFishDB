#include "CVSkipList.h"

int CVSkipList::Put(string key, string value, Iterator iterator){
	t_global_committed.get_and_inc();
    iterator.Put(key,value, iterator);
	return 0;
}


string CVSkipList::Get(string key , Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Seek(key);
    string get_value = iterator.Node()->Get_value();
    return get_value;
}
    


void CVSkipList::RangeQuery(string start_key, int count, Iterator iterator ){
    t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
    cout<<"-----------------------------"<<endl;
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
      for(int i=count; i > 0; --i) {
        cout<<"str_key, str_value = "<<temp_->Get_key()<<", "<<temp_->Get_value()<<endl;
	if(temp_->Next(0)!=nullptr)
       	    temp_=temp_->Next(0);
    } 
    t_global_committed.mlock.unlock();
}



CVSkipList::Splice* CVSkipList::AllocateSplice(){
    /*size_t array_size = sizeof(Node*) * (kMaxHeight_ + 1) + sizeof(Node) ;
    char* raw = new char[sizeof(Splice) + array_size*2];
    Splice* splice = reinterpret_cast<Splice*>(raw);
    splice->height_ = 0;
    splice->prev_ = reinterpret_cast<Node**>(raw + sizeof(Splice) );
    splice->next_ = reinterpret_cast<Node**>(raw + sizeof(Splice) + array_size);
    */
	Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node *[MAX_LEVEL];
	splice->next_ = new Node *[MAX_LEVEL];
    return splice;
}


Node* CVSkipList::FindLast(){
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

Node* CVSkipList::FindLessThan(string key, Node** prev){
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

Node* CVSkipList::FindGreaterorEqual(string key){
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
    


int CVSkipList::RecomputeSpliceLevels(string key, int level, Splice* splice){
    Node* before = head_;
    for(int i =level -1  ;i>=0; --i){
        FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i],before);
    }
	return 0;
}


void CVSkipList::FindSpliceForLevel(string key, int level, Node** sp_prev, Node** sp_next, Node* before){
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

bool CVSkipList::KeyIsAfterNode(string key, Node* n){
  return (n != nullptr) && (key.compare(n->Get_key()) > 0);
} 




Node* CVSkipList::AllocateNode(string key, string value, int height){
   //auto prefix = sizeof(atomic<Node*>) * (height-1);
   //cout<<"prefix = "<<prefix<<endl;
   //char* raw = new char [prefix +sizeof(Node)];
   //cout<<"raw = "<<raw<<endl;
   //printf("raw = %p\n",raw);
   
  //Node* x = reinterpret_cast<Node*>(raw + prefix);   
  //cout<<"x->Get_eky() = "<<x->Get_key()<<endl;
   Node* x = new Node(key,value,height);
	x->done = false;
	
  // for(int i=0;i<height;i ++){
  //     x->SetNext(i,nullptr);
       //assert(x->Next(i));
  // }
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

int CVSkipList::RandomHeight(){
   int height, balancing, pivot;
   balancing =2 ;
   height = 1;
   pivot = 1000/balancing;
   while(height < kMaxHeight_ && height < pivot && (rand()%1000)<pivot){
    height++;
   }
   return height;
}


CVSkipList::CVSkipList()
    :SkipList(static_cast<uint16_t>(MAX_LEVEL), AllocateNode("!","!",MAX_LEVEL),1,AllocateSplice()){
   srand((unsigned)time(NULL));
//   for(int i=0; i<kMaxHeight_;i++){
//        head_->SetNext(i, nullptr);
//   }
//cout<<"-----------------------------------"<<endl;
}


bool CVSkipList::Insert(string key, string value, Iterator iterator)
{
	int height = RandomHeight();
	Node* nnode = AllocateNode(key, value, height);
	//std::deque<T> myreq_q;

	// enqueue a new node 
	pthread_mutex_lock(&req_q.lock);
	req_q.push_back(nnode);

	while(!nnode->done && nnode != req_q.front()){
		pthread_cond_wait(&nnode->cond, &req_q.lock);
	}

	// check if the request is finished 
	if(nnode->done){
		pthread_mutex_unlock(&req_q.lock);
		return true;
	}

#if 0
	// process requset 
	req_q.swap(myreq_q);
#endif
	Node* last_node = req_q.back();
	pthread_mutex_unlock(&req_q.lock);


	while(true){

//		pthread_mutex_lock(&req_q.lock);
		Node* ready_node = req_q.front();
		cout<<"ready_node = "<<ready_node<<endl;
//		pthread_mutex_unlock(&req_q.lock);

		// insert a new node into the skip list 
		int max_height = max_height_.load(std::memory_order_relaxed);
			
		if(ready_node->Get_height() > max_height){
			max_height_ = ready_node->Get_height();
			max_height = ready_node->Get_height();   
 		}
   			
		if(iterator.splice->height_ < max_height){
      		iterator.splice->prev_[max_height] = head_;
      		iterator.splice->next_[max_height] = nullptr;
      		iterator.splice->height_ =max_height;
   		}else{
  			for(int i = 0; i<ready_node->Get_height() ; i++){
	  			iterator.splice->prev_[i] = head_;
	   			iterator.splice->next_[i] = iterator.splice->prev_[i] ->NoBarrier_Next(i);
			}	
  		}

		if(ready_node->Get_height() > 0) {
			RecomputeSpliceLevels(ready_node->Get_key(), ready_node->Get_height(),iterator.splice);
    	}

		for(int i=0;i<ready_node->Get_height();++i){ 
			ready_node->SetNext(i, iterator.splice->next_[i]);
        	iterator.splice->prev_[i]->SetNext(i,ready_node);
    	}
		// insert completes. 
		

		pthread_mutex_lock(&req_q.lock);

		// release ready_node 
	//	assert(ready_node == req_q.front());
		req_q.pop_front();

		// wake up 
		ready_node->done = true;
		pthread_cond_signal(&ready_node->cond);

		pthread_mutex_unlock(&req_q.lock);

		if(ready_node == last_node) {
			break;
		}	
	}

	// make the next thread progress 

	pthread_mutex_lock(&req_q.lock);
	if(!req_q.empty()){
		pthread_cond_signal(&req_q.front()->cond);
	}
	pthread_mutex_unlock(&req_q.lock);

out:
  return true;
}




