#include  "StrideSkipList.h"

int StrideSkipList::Put(string key, string value, Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Put(key,value, iterator);
    return 0;
}


string StrideSkipList::Get(string key, Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Seek(key);
    return iterator.Node()->Get_value();
}



void StrideSkipList::RangeQuery(string start_key, int count, Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
    int i = count;
	if(temp_==nullptr)
		return;
	while(i>1){  
		if(temp_->Get_stride_next()==nullptr){
			if(temp_->Next(0)==nullptr)
				return;
			temp_=temp_->Next(0);
			--i;
		}
		else{
       		temp_=temp_->Get_stride_next();
			--i;
    	}
	} 	
}


Node* StrideSkipList::FindGreaterorEqual(const string& key){
    Node* x = head_;
    int level = kMaxHeight_ -1;
    Node *last_bigger = nullptr;
    
	while(true){
        Node* next = x->Next(level);
		GET_REFERENCE(level);
		COUNT(cnt);
        int cmp = (next == nullptr || next == last_bigger) ? -1 : KeyIsAfterNode(key,next);
       
		 if(cmp <= 0 &&level ==0){
            GET_LEVEL(level);
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
 


StrideSkipList::Splice* StrideSkipList::AllocateSplice(){
	Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node *[MAX_LEVEL];
	splice->next_ = new Node *[MAX_LEVEL];
   return splice;
}

int StrideSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice){
   
	// head 
	int i = MAX_LEVEL-1;
	int cmp;
	Node* start = head_;

	while(1) {
		cmp = FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i],start);
		if(i<=to_level)
			break;
		--i;
		start = splice->prev_[i+1];
	}
	PUT_LEVEL(0);
	return -1;
}


int StrideSkipList::FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before)
{
	assert(before != NULL);
	int cmp;

    Node* after = before->Next(level);
	COUNT(pointer_cnt);
	while(true){
		PUT_REFERENCE(level);	
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



int StrideSkipList::KeyIsAfterNode(const string& key, Node* n){
	if(n == nullptr)
		return -1;

	//cpr_cnt++;
	return key.compare(n->Get_key());
}


Node* StrideSkipList::AllocateNode(const string& key, const string& value, int height){
   Node* x = new Node(key,value, height);
   return x;
}

bool StrideSkipList::Insert(string key, string value, Iterator iterator){
  //Node* nnode = AllocateNode(key, value, RandomHeight());
//  Splice* splice = AllocateSplice();
	int height = RandomHeight();  
#ifdef PRINT_HEIGHT
	cout << height << endl;
#endif
	int max_height = max_height_.load(std::memory_order_relaxed);

	while(height > max_height){
//		height = max_height+1;
		if(max_height_.compare_exchange_weak(max_height, height)){
			//max_height = height; // EUNJI: what's this? 
			break;
		}
	}
	
	Node* nnode = AllocateNode(key, value, height);

	int rv = RecomputeSpliceLevels(key, 0,iterator.splice);
    assert(rv < 0);
	for(int i=0; i<height ; i++){
		while(true){
			nnode->NoBarrier_SetNext(i, iterator.splice->next_[i]);
			COUNT(pointer_cnt);
			if(iterator.splice->prev_[i]->CASNext(i,iterator.splice->next_[i],nnode)){
				COUNT(CAS_cnt);
				//success
				break;
			}
			COUNT(CAS_failure_cnt);			
			rv = RecomputeSpliceLevels(nnode->Get_key(), i,iterator.splice);
			assert(rv < 0);
		}
	}
	SET_LEVEL(height-1);
	
	if(iterator.splice->next_[0] != nullptr && iterator.splice->next_[0]->Get_key() == key)
	{	
		nnode->Set_stride_next(iterator.splice->next_[0]);
	}
   return true;
     
}

void StrideSkipList::PrintReference(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] SR PUT_SEEK = " << i << " " << PUT_REFERENCE[i] <<endl; 
	}

	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] SR GET_SEEK = " << i << " " << GET_REFERENCE[i] <<endl; 
	}
}

void StrideSkipList::PrintLevel(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] SR PUT_LEVEL = " << i << " " << PUT_LEVEL[i] <<endl; 
	}

	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] SR GET_LEVEL = " << i << " " << GET_LEVEL[i] <<endl; 
	}
}

void StrideSkipList::PrintSetLevel(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] SR SET_LEVEL = " << i << " " << SET_LEVEL[i] <<endl;
	}
}

void StrideSkipList::PrintStat()
{
//	cout << "StrideSkipList comparator count = " << cnt << endl;
	cout << "[STAT] SR CAS = "<< CAS_cnt << endl;
//	cout << "StrideSkipList pointer update count = " << pointer_cnt << endl;
	cout << "[STAT] SR CAS_FAIL = " << CAS_failure_cnt << endl;
}

void StrideSkipList::ResetStat()
{
	cpr_cnt = 0;
}


StrideSkipList::StrideSkipList()
{
	string key = "!";
	string val = "!";
	head_ = AllocateNode(key, val, MAX_LEVEL); 
	kMaxHeight_ = MAX_LEVEL;	
	seq_splice = AllocateSplice(); 

    srand((unsigned)time(NULL));
	cpr_cnt = 0;
}
