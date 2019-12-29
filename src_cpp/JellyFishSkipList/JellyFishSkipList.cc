#include "JellyFishSkipList.h"

int JellyFishSkipList::Put(string key, string value, Iterator iterator){
#ifdef OP_EXEC
    t_global_committed.get_and_inc();
    iterator.Put(key, value, iterator);
#endif
    return 0;
}

#if 1
string JellyFishSkipList::Get(string key, Iterator iterator){
#ifdef OP_EXEC
    t_global_committed.get_and_inc();
	iterator.Seek(key);
	if(iterator.Node() -> Get_vqueue() != nullptr){
		return iterator.Node() -> Get_vqueue() -> value; 
    }
    else	
		return iterator.Node()->Get_value();
#else
	string val("deadbeaf");
	return val; 
#endif
}
#endif


void JellyFishSkipList::RangeQuery(string start_key, int count, Iterator iterator){
    t_global_committed.get_and_inc();
//    cout<<"-----------------------------"<<endl;
	iterator.Seek(start_key);
	Node* temp_ = iterator.Node();
	int i = count;
	if(temp_ == nullptr)
		return;
	while(i > 1){
		GET_REFERENCE(0);
      //for(int i=count; i > 0; --i) {
		if(temp_->Next(0)==nullptr)
			return;
		if(temp_->Get_vqueue() != nullptr){
			--i;
		}
		else{
			--i;
		}
       	temp_=temp_->Next(0);
//		--i;
    } 
	return;
}

#if 1
Node* JellyFishSkipList::FindGreaterorEqual(const string& key){
    Node* x = head_;
    int level = max_height_ -1;
    Node *last_bigger = nullptr;

    while(true){

        Node* next = x->Next(level);
		GET_REFERENCE(level);
		COUNT(cnt);
		int cmp = (next == nullptr || next == last_bigger) ? -1 : KeyIsAfterNode(key,next);
		
		if(cmp==0){
			GET_LEVEL(level);
			return next;
		}
		else if(cmp < 0 && level ==0){
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
#endif


JellyFishSkipList::Splice* JellyFishSkipList::AllocateSplice(){
	
	Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node *[MAX_LEVEL];
	splice->next_ = new Node *[MAX_LEVEL];
    return splice;
}


int JellyFishSkipList::KeyIsAfterNode(const string& key, Node* n)
{
	if(n == nullptr)
		return -1;

	//cpr_cnt++;
	return key.compare(n->Get_key());
}


//void JellyFishSkipList::FindSpliceForLevel(const string& key, int level,  Node** sp_prev, Node** sp_next, Node* before)
int JellyFishSkipList::FindSpliceForLevel(const string& key, int level,  Node** sp_prev, Node** sp_next, Node* before)
{
	assert(before != NULL);
	int cmp = 1;

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


int  JellyFishSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice)
{
	// head 
	int i = max_height_-1;
	int cmp; 
	Node* start = head_;

	while(1){
		cmp = FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], start);

		// check if the value is found 
#if 1		
		if(cmp == 0){
			PUT_LEVEL(i);
			return i;
		}
#endif
		// continue searching 
		if(i <= to_level)
			break; 
		--i; 
		start = splice->prev_[i+1];
	}
	PUT_LEVEL(0);
	return -1;
}





VNode* JellyFishSkipList::AllocateVNode(const string& value){
   VNode* x = new VNode;
   x->value = value;
   x->NoBarrier_SetNext(nullptr);
   //x->timestamp = n->Get_vqueue_num();
   //x->next = n->Get_vqueue();
   return x; 
}


Node* JellyFishSkipList::AllocateNode(const string& key, const string& value, int height){
   Node* x = new Node(key, value, height);
   return x;
}


bool JellyFishSkipList::Insert(string key, string value, Iterator iterator)
{
	int height = RandomHeight();
#ifdef PRINT_HEIGHT
	cout << height << endl;
#endif
	// fl : found_level
	int max_height = max_height_.load(std::memory_order_relaxed);

	while(height > max_height){
		if(max_height_.compare_exchange_weak(max_height, height)){
			//max_height = height; // EUNJI: what's this? 
			break;
		}
	}
	int fl = RecomputeSpliceLevels(key, 0, iterator.splice);
#ifdef JELLYFISH
	// Insert a new node into the value chain 
	if(fl >= 0){ 
found:
		Node* fnode = iterator.splice->next_[fl];
		VNode* nvnode = AllocateVNode(value);
		DEBUG("nvnode = " << nvnode);


		while(1) {
			// first node 
			VNode* vq = fnode->Get_vqueue();
		
			if(vq == nullptr) { // empty, and it's the first node 
				if(fnode->CAS_vqueue(vq, nvnode)){
					COUNT(CAS_cnt);
					return true;
				}
				COUNT(CAS_failure_cnt);
				continue;
			}

			// not first node
			assert(fnode->Get_key() == key);
			DEBUG("Insert into vqueue: key = " + key);
			DEBUG(" vq = " << vq );
			nvnode->NoBarrier_SetNext(vq->NoBarrier_Next());
			COUNT(pointer_cnt);
			if(fnode->CAS_vqueue(vq, nvnode)){
				COUNT(CAS_cnt);
				return true;
			}
			COUNT(CAS_failure_cnt);
		}
	}
#endif
	// Insert a new node into the skip list 
	Node* nnode = AllocateNode(key, value, height);
		
	for(int i = 0; i < height; ++i){
		while(true){
			nnode -> NoBarrier_SetNext(i, iterator.splice->next_[i]);
			COUNT(pointer_cnt);
			if(iterator.splice->prev_[i]->CASNext(i, iterator.splice->next_[i], nnode)){
				// success
				COUNT(CAS_cnt); 
				break;
	   		}
			
			// failure 
			COUNT(CAS_failure_cnt);	
			fl = RecomputeSpliceLevels(key, i, iterator.splice);
#ifdef JELLYFISH
			// if an insert fails in a bottom layer and a new node with a same key 
			// is already inserted, we should insert the node into a value chain. 
			if (i == 0 && fl >= 0){
				if(nnode) free(nnode);
				goto found;
			}
#endif
		}
	}
	
	SET_LEVEL(height-1);
	return true; 
}

void JellyFishSkipList::PrintReference(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] JF PUT_SEEK = " << i << " " << PUT_REFERENCE[i] <<endl; 
	}

	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] JF GET_SEEK = " << i << " " << GET_REFERENCE[i] <<endl; 
	}
}

void JellyFishSkipList::PrintSetLevel(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] JF SET_LEVEL = " << i << " " << SET_LEVEL[i] <<endl; 
	}
}

void JellyFishSkipList::PrintLevel(){
	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] JF PUT_LEVEL = " << i << " " << PUT_LEVEL[i] <<endl; 
	}

	for(int i = MAX_LEVEL-1 ; i >= 0 ; --i){
		cout<< "[STAT] JF GET_LEVEL = " << i << " " << GET_LEVEL[i] <<endl; 
	}
}
void JellyFishSkipList::PrintStat()
{
//	cout << "JellyFishSkipList comparator count = " << cnt << endl;
	cout << "[STAT] JF CAS = "<< CAS_cnt << endl;
//	cout << "JellyFishSkipList pointer update count = " << pointer_cnt << endl;
	cout << "[STAT] JF CAS_FAIL = " << CAS_failure_cnt << endl;
}
void JellyFishSkipList::ResetStat()
{
	cpr_cnt = 0;
}


JellyFishSkipList::JellyFishSkipList()
{
	string key = "!";
	string val = "!";
	head_ = AllocateNode(key, val, MAX_LEVEL); 
	kMaxHeight_ = MAX_LEVEL;	
	seq_splice = AllocateSplice(); 

    srand((unsigned)time(NULL));
	cpr_cnt = 0;
}
