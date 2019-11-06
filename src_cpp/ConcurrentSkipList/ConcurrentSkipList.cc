#include  "ConcurrentSkipList.h"

int ConcurrentSkipList::Put(string key, string value, Iterator iterator){
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
	while(i>1){
		if(temp_->Next(0)==nullptr)
			return;
		if(temp_->Next(0)->Get_key() != temp_->Get_key()){
			--i;
		}
		else{
		//	++cnt;
		}
       	temp_=temp_->Next(0);
    } 	
}


Node* ConcurrentSkipList::FindGreaterorEqual(const string& key){
    Node* x = head_;
    int level = kMaxHeight_ -1;
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
	int i = MAX_LEVEL-1;
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



Node* ConcurrentSkipList::AllocateNode(const string& key, const string& value, int height){
   Node* x = new Node(key, value, height);
   return x;
}



bool ConcurrentSkipList::Insert(string key, string value, Iterator iterator)
{
	// update current max height
	int height = RandomHeight();
#ifdef PRINT_HEIGHT
	cout << height << endl;
#endif

	// Allocate a new node 
	Node* nnode = AllocateNode(key, value, height);

	int rv = RecomputeSpliceLevels(key, 0, iterator.splice);
	//int rv = RecomputeSpliceLevels(nnode->Get_key(), nnode->Get_height(),0,iterator.splice);
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
	
	return true;
}

void ConcurrentSkipList::PrintStat()
{
	cout << "ConcurrentSkipList comparator count = " << cnt << endl;
	cout << "ConcurrentSkipList CAS count = "<< CAS_cnt << endl;
	cout << "ConcurrentSkipList pointer update count = " << pointer_cnt << endl;
	cout << "ConcurrentSkipList CAS failure count = " << CAS_failure_cnt << endl; 
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



