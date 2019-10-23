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
//    t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
//    cout<<"-----------------------------"<<endl;
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
	int i = count;
	while(i>0){
		if(temp_->Next(0)==nullptr)
			return;
		if(temp_->Next(0)->Get_key() != temp_->Get_key())
			--i;
       	temp_=temp_->Next(0);
     } 
//    t_global_committed.mlock.unlock();
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

Node* CVSkipList::FindLessThan(const string& key, Node** prev){
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

Node* CVSkipList::FindGreaterorEqual(const string& key){
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
    


int CVSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice){
 	// head 
	int i = MAX_LEVEL-1;
	FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], head_);
	while(i > to_level) {
		--i;
		FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], splice->prev_[i+1]);
	}
	return -1; 
}

void CVSkipList::FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before){
	assert(before != nullptr);
	Node* after = before ->Next(level);
	while(true){
        if(!KeyIsAfterNode(key, after)){
			*sp_prev = before;
            *sp_next = after;
            return;
        }
        	before = after;
            after = after->Next(level);
    }
}

bool CVSkipList::KeyIsAfterNode(const string& key, Node* n){
	if(n == nullptr)
		return false;
	cpr_cnt++;
	return key.compare(n->Get_key()) > 0;
}





Node* CVSkipList::AllocateNode(const string& key,const string& value, int height){
   Node* x = new Node(key,value,height);
	x->done = false;
	pthread_cond_init(&x->cond,NULL);	
   return x;
}
 


bool CVSkipList::Insert(string key, string value, Iterator iterator)
{
	int height = RandomHeight();
	Node* nnode = AllocateNode(key, value, height);
	//std::deque<T> myreq_q;
	// enqueue a new node
	pthread_mutex_lock(&req_q.lock);
	req_q.push_back(nnode);

#if 1
	while(nnode != req_q.front()){
		pthread_cond_wait(&nnode->cond, &req_q.lock);
		if(nnode->done){
			pthread_mutex_unlock(&req_q.lock);
			return true;
		}
	}
#endif
	// check if the request is finished 
#if 0	
	if(nnode->done){
		cout<<"return ( node = "<<nnode<<" ) "<<endl;
		pthread_mutex_unlock(&req_q.lock);
		return true;
	}
#endif

	pthread_mutex_unlock(&req_q.lock);
	
	Node* last_node = req_q.back();

	while(true){
		Node* ready_node = req_q.front();
//		pthread_mutex_unlock(&req_q.lock);

		// insert a new node into the skip list 
		int max_height = max_height_.load(std::memory_order_relaxed);
		if(ready_node->Get_height() > max_height){
			max_height_ = ready_node->Get_height();
			max_height = ready_node->Get_height();   
 		}

		int rv = RecomputeSpliceLevels(ready_node->Get_key(), 0, iterator.splice);
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
	return true;
}


void CVSkipList::PrintStat()
{
	cout << "CVSkipList comparator count = " << cpr_cnt << endl;

}
void CVSkipList::ResetStat()
{
	cpr_cnt = 0;
}

CVSkipList::CVSkipList()
{
	string key = "!";
	string val = "!";
	head_ = AllocateNode(key, val, MAX_LEVEL); 
	kMaxHeight_ = MAX_LEVEL;	
	max_height_ = 1; 
	seq_splice = AllocateSplice(); 
	pthread_mutex_init(&req_q.lock,NULL);
    srand((unsigned)time(NULL));
	cpr_cnt = 0;
}

