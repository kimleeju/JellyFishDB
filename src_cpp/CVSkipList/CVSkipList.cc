#include "CVSkipList.h"

int CVSkipList::Put(string key, string value, Iterator iterator){
	t_global_committed.get_and_inc();
    iterator.Put(key,value, iterator);
	return 0;
}


string CVSkipList::Get(string key , Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Seek(key);
    return iterator.Node()->Get_value();

}
    


void CVSkipList::RangeQuery(string start_key, int count, Iterator iterator ){
	t_global_committed.get_and_inc();
	iterator.Seek(start_key);
	Node* temp_ = iterator.Node();
	int i = count;
	
	if(temp_== nullptr)
		return;
	string str_key(temp_->Get_key());
	while(i>1){
		if(temp_->Next(0)==nullptr)
			return;
		if(temp_->Next(0)->Get_key() != str_key){
			str_key = temp_->Next(0)->Get_key();
			--i;
		}
		else{
		//	++cnt;
		}
       	temp_=temp_->Next(0);
    } 	
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

Node* CVSkipList::FindGreaterorEqual(const string& key){
    Node* x = head_;
    int level = max_height_ -1;
    Node *last_bigger = nullptr;
    while(true){
        Node* next = x->Next(level);
        COUNT(cnt);
		int cmp = (next == nullptr || next == last_bigger) ? -1 : KeyIsAfterNode(key,next);

        if(cmp <= 0 &&level ==0){
            return next;
        }
        else if (cmp > 0){
            x= next;
        }
        else{
            last_bigger = next;
            level --;
        }
    }
}
    


int CVSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice){

// head 
	int i = max_height_-1;
	int cmp; 
	Node* start = head_;

	while(1){
		cmp = FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], start);
		// continue searching 
		if(i <= to_level)
			break; 
		--i; 
		start = splice->prev_[i+1];
	}
	return -1;

}

int CVSkipList::FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before){
	

	assert(before != NULL);
	int cmp;

    Node* after = before->Next(level);
	COUNT(pointer_cnt);
	while(true){
		if(after) 
			cmp = KeyIsAfterNode(key, after);	

		if(!after || cmp <= 0) {
			COUNT(pointer_cnt);
			COUNT(pointer_cnt);
			*sp_prev = before;
			*sp_next = after;
			return cmp;
		}
		COUNT(pointer_cnt);
        COUNT(pointer_cnt);
		before = after;
        after = after->Next(level);
	}
}

int CVSkipList::KeyIsAfterNode(const string& key, Node* n){
	if(n == nullptr)
		return -1;
	//cpr_cnt++;
	return key.compare(n->Get_key());
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

	pthread_mutex_unlock(&req_q.lock);
	
	Node* last_node = req_q.back();

	pthread_mutex_lock(&req_q.lock);
	while(true){
		Node* ready_node = req_q.front();
		pthread_mutex_unlock(&req_q.lock);
		// insert a new node into the skip list
		
		if(ready_node->Get_height() > max_height_){
			max_height_ = ready_node->Get_height();
    	}

		int rv = RecomputeSpliceLevels(ready_node->Get_key(), 0, iterator.splice);
		for(int i=0;i<ready_node->Get_height();++i){ 
			ready_node->SetNext(i, iterator.splice->next_[i]);
        	iterator.splice->prev_[i]->SetNext(i,ready_node);
		
			COUNT(pointer_cnt);
			COUNT(pointer_cnt);	
		}
		// insert completes. 
		// release ready_node 
		pthread_mutex_lock(&req_q.lock);
		req_q.pop_front();
		// wake up
//		pthread_mutex_lock(&req_q.lock);
		ready_node->done = true;	
		pthread_cond_signal(&ready_node->cond);
//		pthread_mutex_unlock(&req_q.lock);
		if(ready_node == last_node) {
			break;
		}
	}
	// make the next thread progress 
//	pthread_mutex_lock(&req_q.lock);
	if(!req_q.empty()){
		pthread_cond_signal(&req_q.front()->cond);
	}
	pthread_mutex_unlock(&req_q.lock);
	return true;
}


void CVSkipList::PrintStat()
{
	cout << "BlockedCVSkipList comparator count = " << cnt << endl;
	cout << "BlockedCVSkipList CAS count = "<< CAS_cnt << endl;
	cout << "BlockedCVSkipList pointer update count = " << pointer_cnt << endl;
	cout << "BlockedCVSkipList CAS failure count = " << CAS_failure_cnt << endl;

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
	seq_splice = AllocateSplice(); 
	pthread_mutex_init(&req_q.lock,NULL);
    srand((unsigned)time(NULL));
	cpr_cnt = 0;
}
