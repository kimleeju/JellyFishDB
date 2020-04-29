#include  "LinkedSkipList.h"

int LinkedSkipList::Put(string key, string value, Iterator_Linked& iterator){
#ifdef OP_EXEC
	t_global_committed.get_and_inc();
	iterator.Put(key,value, iterator);
#endif
    return 0;
}


string LinkedSkipList::Get(string key, Iterator_Linked iterator){
#ifdef OP_EXEC
    t_global_committed.get_and_inc();
    iterator.Seek(key);
    return iterator.Node_Linked()->Get_value();

#else
	string get_value("deadbeaf");
	return get_value;
#endif
}



void LinkedSkipList::RangeQuery(string start_key, int count, Iterator_Linked iterator){
	t_global_committed.get_and_inc();
	iterator.Seek(start_key);
	Node_Linked* temp_ = iterator.Node_Linked();
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


Node_Linked* LinkedSkipList::FindGreaterorEqual(const string& key){
    Node_Linked* x = head_;
    int level = max_height_ -1;
    Node_Linked *last_bigger = nullptr;
	// EUNJI_TEST
	//int test; 

    while(true){
        Node_Linked* next = x->Next(level);
		GET_REFERENCE(level);
		COUNT(cnt);
        int cmp = (next == nullptr || next == last_bigger) ? -1 : KeyIsAfterNode(key,next);
     
#if 0  
		if(cmp ==0){
			test = cmp;
		}
#endif
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
 

LinkedSkipList::Splice* LinkedSkipList::AllocateSplice(){
	Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node_Linked *[MAX_LEVEL];
	splice->next_ = new Node_Linked *[MAX_LEVEL];
  	return splice;
}

int LinkedSkipList::KeyIsAfterNode(const string& key, Node_Linked* n){
	if(n == nullptr)
		return -1;

	//cpr_cnt++;
	return key.compare(n->Get_key());
}


int LinkedSkipList::FindSpliceForLevel(const string& key, int level, 
		Node_Linked** sp_prev, Node_Linked** sp_next, Node_Linked* before)
{
	
	assert(before != NULL);
	int cmp;

	Node_Linked* after = before->Next(level);
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

int LinkedSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice)
{
	//head
	int i = max_height_-1;
	int cmp; 
	Node_Linked* start = head_;

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



Node_Linked* LinkedSkipList::AllocateNode(Iterator_Linked& iterator, const string& key, const string& value, int height){
	Node_Linked* x = new Node_Linked(iterator.arena,key, value, height);
	return x;
}



Node_Linked* LinkedSkipList::AllocateNode(const string& key, const string& value, int height){
	Node_Linked* x = new Node_Linked(key, value, height);
	return x;
}



bool LinkedSkipList::Insert(string key, string value, Iterator_Linked& iterator)
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
	
//	Node_Linked* nnode = AllocateNode(key, value, height);
	Node_Linked* nnode = AllocateNode(iterator, key, value, height);
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
void LinkedSkipList::PrintReference(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] LK PUT_SEEK = " << i << " " << PUT_REFERENCE[i] <<endl; 
	}

	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] LK GET_SEEK = " << i << " " << GET_REFERENCE[i] <<endl; 
	}
}

void LinkedSkipList::PrintLevel(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] LK PUT_LEVEL = " << i << " " << PUT_LEVEL[i] <<endl; 
	}

	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] LK GET_LEVEL = " << i << " " << GET_LEVEL[i] <<endl; 
	}
}

void LinkedSkipList::PrintSetLevel(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] LK SET_LEVEL = " << i << " " << SET_LEVEL[i] <<endl;
	}
}

void LinkedSkipList::PrintStat()
{
//	cout << "LinkedSkipList comparator count = " << cnt << endl;
	cout << "[STAT] LK CAS = "<< CAS_cnt << endl;
//	cout << "LinkedSkipList pointer update count = " << pointer_cnt << endl;
	cout << "[STAT] LK CAS_FAIL = " << CAS_failure_cnt << endl;
}

void LinkedSkipList::ResetStat()
{
	cpr_cnt = 0;
}


LinkedSkipList::LinkedSkipList()
{
	string key = "!";
	string val = "!";
	head_ = AllocateNode(key, val, MAX_LEVEL); 
	kMaxHeight_ = MAX_LEVEL;	
	seq_splice = AllocateSplice(); 

    srand((unsigned)time(NULL));
	cpr_cnt = 0;
}



