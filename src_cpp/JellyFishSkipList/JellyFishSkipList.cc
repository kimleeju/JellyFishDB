#include "JellyFishSkipList.h"

int JellyFishSkipList::Put(string key, string value, Iterator iterator){
#ifdef OP_EXEC
    t_global_committed.get_and_inc();
    iterator.Put(key, value, iterator);
#endif
    return 0;
}


string JellyFishSkipList::Get(string key, Iterator iterator){
#ifdef OP_EXEC
    t_global_committed.get_and_inc();
	iterator.Seek(key);
	Node* temp = iterator.Node();
	  if(temp -> Get_vqueue_num() != 0){
		return temp -> Get_vqueue() -> value; 
    }
    else	
		return temp->Get_value();
#else
	string val("deadbeaf");
	return val; 
#endif
}



void JellyFishSkipList::RangeQuery(string start_key, int count, Iterator iterator){
    t_global_committed.get_and_inc();
//    cout<<"-----------------------------"<<endl;
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
	int i = count;
	while(i > 0){
      //for(int i=count; i > 0; --i) {
		if(temp_->Get_vqueue_num() != 0){
			--i;
		}
		else{
			--i;
		}
		if(temp_->Next(0)==nullptr)
			return;
       	temp_=temp_->Next(0);
		--i;
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

Node* JellyFishSkipList::FindLessThan(const string& key, Node** prev){
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

Node* JellyFishSkipList::FindGreaterorEqual(const string& key){
    Node* x = head_;
    int level = kMaxHeight_ -1;
    Node *last_bigger = nullptr;
    while(true){

        Node* next = x->Next(level);
		int cmp = (next == nullptr || next == last_bigger) ? 1 : KeyIsAfterNode(key,next);
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
	
	Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node *[MAX_LEVEL];
	splice->next_ = new Node *[MAX_LEVEL];
    return splice;
}


bool JellyFishSkipList::KeyIsAfterNode(const string& key, Node* n)
{
	if(n == nullptr)
		return false;

	cpr_cnt++;
	return key.compare(n->Get_key()) > 0;
}


void JellyFishSkipList::FindSpliceForLevel(const string& key, int level,  Node** sp_prev, Node** sp_next, Node* before)
{
	assert(before != NULL);

    Node* after = before->Next(level);

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


int  JellyFishSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice)
{
	// head 
	int i = MAX_LEVEL-1;
	FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], head_);

	while(i > to_level) {
		--i;
		FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], splice->prev_[i+1]);
		if(splice->next_[i] && (Comparator(key, splice->next_[i]->Get_key())==0)){
			DEBUG( __func__ << " " << key << " " << splice->next_[i]->Get_key());
			return i;	
		}
	}
	return -1;
}



int JellyFishSkipList::Comparator(string key1, string key2){
  return key1.compare(key2);
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

int JellyFishSkipList::RandomHeight(){

	int height = 1;
	
	int rnum = rand();

	if(rnum & 0x3) { // 둘다 0 이어야 동작. 
		while(rnum & 1 << 30 && height < kMaxHeight_) {
			height++;
			rnum <<= 1;
		} 
	 }
	return height;
}

bool JellyFishSkipList::Insert(string key, string value, Iterator iterator)
{
	int height = RandomHeight();

#ifdef PRINT_HEIGHT
	cout << height << endl;
#endif
	int max_height = max_height_.load(std::memory_order_relaxed);
	while(height > max_height){
		if(max_height_.compare_exchange_weak(max_height, height)){
			max_height = height;
			break;
		}
	}

	// fl : found_level
	int fl = RecomputeSpliceLevels(key, 0, iterator.splice);

#ifdef JELLYFISH
	// Insert a new node into the value chain 
	if(fl >= 0){ 
found:
		Node* fnode = iterator.splice->next_[fl];
		VNode* nvnode = AllocateVNode(value);


		while(1) {
			// first node 
			VNode* vq = fnode->Get_vqueue();
	
			if(vq == nullptr) {
				if(fnode->CAS_vqueue(vq, nvnode))
					return true;
			}
	
			// not first node  
			nvnode->NoBarrier_SetNext(vq->NoBarrier_Next());
	
			if(fnode->CAS_vqueue(vq, nvnode))
				return true;
		}
	}
#endif

	// Insert a new node into the skip list 
	Node* nnode = AllocateNode(key, value, height);
		
	for(int i = 0; i < height; ++i){
		while(true){
			nnode -> NoBarrier_SetNext(i, iterator.splice->next_[i]);

			if(iterator.splice->prev_[i]->CASNext(i, iterator.splice->next_[i], nnode)){
				// success 
				break;
	   		}
			
			// failure 	
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
	return true; 
}


void JellyFishSkipList::PrintStat()
{
	cout << "JellyFishSkipList comparator count = " << cpr_cnt << endl;

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
	max_height_ = 1; 
	seq_splice = AllocateSplice(); 

    srand((unsigned)time(NULL));
	cpr_cnt = 0;
}
