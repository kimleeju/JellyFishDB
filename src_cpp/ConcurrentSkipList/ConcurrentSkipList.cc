#include  "ConcurrentSkipList.h"

std::atomic<int> i;


int ConcurrentSkipList::Put(string key, string value, Iterator& iterator){
#ifdef OP_EXEC
	t_global_committed.get_and_inc();
	iterator.Put(key,value, iterator);
#endif
    return 0;
}


string ConcurrentSkipList::Get(string key, Iterator iterator){
#ifdef OP_EXEC
	t_global_committed.get_and_inc();
	iterator.Seek(key);
	return iterator.Node()->Get_value();

#else
	string get_value("deadbeaf");
	return get_value;
#endif
}



void ConcurrentSkipList::RangeQuery(string start_key, int count, Iterator iterator){
	t_global_committed.get_and_inc();
	iterator.Seek(start_key);
	Node* temp_ = iterator.Node();
	int i = count;
	
	if(temp_== nullptr)
		return;
#if 1
	string str_key(temp_->Get_key());
	while(i>1){
		GET_REFERENCE(0);
		if(temp_->Next(0)==nullptr)
			return;

		if(temp_->Next(0)->Get_key() != str_key){
			str_key = temp_->Next(0)->Get_key();
			--i;
		}
       	temp_=temp_->Next(0);
    }
#endif 	
}


Node* ConcurrentSkipList::FindGreaterorEqual(const string& key){
	Node* x = head_;
	int level = max_height_ -1;
	Node *last_bigger = nullptr;
	// EUNJI_TEST
	//int test; 

	Node* next;
	while(true){
		next = x->Next(level);
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
 

ConcurrentSkipList::Splice* ConcurrentSkipList::AllocateSplice(){
	Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node *[MAX_LEVEL];
	splice->next_ = new Node *[MAX_LEVEL];
  	return splice;
}

int ConcurrentSkipList::KeyIsAfterNode(const string& key, Node* n){
	if(n == nullptr)
		return -1;

	//cpr_cnt++;
	return key.compare(n->Get_key());
}


int ConcurrentSkipList::FindSpliceForLevel(const string& key, int level, 
		Node** sp_prev, Node** sp_next, Node* before)
{
	
	assert(before != NULL);
	int cmp;

	Node* after = before->Next(level);
	COUNT(pointer_cnt);
	while(true){
		PUT_REFERENCE(level);	
		//if(after == NULL)
			//cout<<"aaaaaaaa-------->";
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

int ConcurrentSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice)
{
	//head
	int i = max_height_-1;
	int cmp = 1; 
	Node* start = head_;

	while(1){
		cmp = FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], start);
		// continue searching 
		if(i <= to_level)
			break; 
		--i; 
		start = splice->prev_[i+1];
	}
	
	PUT_LEVEL(0);
	return -1;

}


#if 1
Node* ConcurrentSkipList::AllocateNode(Iterator& iterator, const string& key, const string& value, int height){
	Node* x = new Node(iterator.arena,key, value, height);
	return x;
}

#endif


Node* ConcurrentSkipList::AllocateNode(const string& key, const string& value, int height){
	Node* x = new Node(key, value, height);
	return x;
}



bool ConcurrentSkipList::Insert(string key, string value, Iterator& iterator)
{
#if 1
	// update current max height
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
	// Allocate a new node 
	
	Node* nnode = AllocateNode(key, value, height);
//	Node* nnode = AllocateNode(iterator, key, value, height);
//	cout<<"i = "<<++i<<endl;
	int rv = RecomputeSpliceLevels(key, 0, iterator.splice);
	assert(rv < 0);

	for(int i=0; i < height; i++){
		while(true){
			nnode->NoBarrier_SetNext(i, iterator.splice->next_[i]);	
			COUNT(pointer_cnt);
			if(iterator.splice->prev_[i]->CASNext(i,iterator.splice->next_[i],nnode)){
				COUNT(CAS_cnt);
				break; // success 
			}	
			// failure
			COUNT(CAS_failure_cnt);
			rv = RecomputeSpliceLevels(key, i, iterator.splice);
			assert(rv < 0);
		}
	}
	SET_LEVEL(height-1);
	return true;
#endif
}
void ConcurrentSkipList::PrintReference(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] CC PUT_SEEK = " << i << " " << PUT_REFERENCE[i] <<endl; 
	}

	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] CC GET_SEEK = " << i << " " << GET_REFERENCE[i] <<endl; 
	}
}

void ConcurrentSkipList::PrintLevel(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] CC PUT_LEVEL = " << i << " " << PUT_LEVEL[i] <<endl; 
	}

	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] CC GET_LEVEL = " << i << " " << GET_LEVEL[i] <<endl; 
	}
}

void ConcurrentSkipList::PrintSetLevel(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] CC SET_LEVEL = " << i << " " << SET_LEVEL[i] <<endl;
	}
}

void ConcurrentSkipList::PrintStat()
{
//	cout << "ConcurrentSkipList comparator count = " << cnt << endl;
	cout << "[STAT] CC CAS = "<< CAS_cnt << endl;
//	cout << "ConcurrentSkipList pointer update count = " << pointer_cnt << endl;
	cout << "[STAT] CC CAS_FAIL = " << CAS_failure_cnt << endl;
}

void ConcurrentSkipList::ResetStat()
{
	cpr_cnt = 0;
}


ConcurrentSkipList::ConcurrentSkipList()
{
	string key = "!";
	string val = "!";
	head_ = AllocateNode(key, val, MAX_LEVEL); 
	kMaxHeight_ = MAX_LEVEL;	
	seq_splice = AllocateSplice(); 

    srand((unsigned)time(NULL));
	cpr_cnt = 0;
}



