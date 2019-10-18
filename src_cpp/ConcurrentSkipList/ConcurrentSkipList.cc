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
    string get_value = iterator.Node()->Get_value();
    return get_value;
#else
	string get_value("deadbeaf");
	return get_value;
#endif
}



void ConcurrentSkipList::RangeQuery(string start_key, int count, Iterator iterator){
    t_global_committed.get_and_inc();
//    cout<<"-----------------------------"<<endl;
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
    //  for(int i=count; i > 0; --i) {
	int i = count;
	while(i>0){
		if(temp_->Next(0)==nullptr)
			return;
		if(temp_->Next(0)->Get_key() != temp_->Get_key()){
			//cout<<temp_->Get_key()<<" -> ";
			--i;
		}
		else{
		//	++cnt;
		}
       	temp_=temp_->Next(0);
    } 	
//	cout<<endl;
}

Node* ConcurrentSkipList::FindLast(){
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

Node* ConcurrentSkipList::FindLessThan(const string& key, Node** prev){
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

Node* ConcurrentSkipList::FindGreaterorEqual(const string& key){
    Node* x = head_;
    int level = kMaxHeight_ -1;
    Node *last_bigger = nullptr;
    while(true){
        Node* next = x->Next(level);
        int cmp = (next == nullptr || next == last_bigger) ? 1 :Comparator(next->Get_key(),key);
       
		 if(cmp >= 0 &&level ==0){
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
 

ConcurrentSkipList::Splice* ConcurrentSkipList::AllocateSplice(){
	Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node *[MAX_LEVEL];
	splice->next_ = new Node *[MAX_LEVEL];
   return splice;
}

bool ConcurrentSkipList::KeyIsAfterNode(const string& key, Node* n){
	if(n == nullptr)
		return false;

	cpr_cnt++;
	return key.compare(n->Get_key()) > 0;
}


void ConcurrentSkipList::FindSpliceForLevel(const string& key, int level, 
		Node** sp_prev, Node** sp_next, Node* before)
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

int ConcurrentSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice)
{
	// head 
	int i = MAX_LEVEL-1;
	FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], head_);

	while(i > to_level) {
		--i;
		FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], splice->prev_[i+1]);
	}
	return -1;
}


int ConcurrentSkipList::Comparator(string key1, string key2){
	return key1.compare(key2);
}

Node* ConcurrentSkipList::AllocateNode(const string& key, const string& value, int height){
   Node* x = new Node(key, value, height);
   return x;
}

int ConcurrentSkipList::RandomHeight()
{
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



bool ConcurrentSkipList::Insert(string key, string value, Iterator iterator)
{
	// update current max height
	int height = RandomHeight();

	int max_height = max_height_.load(std::memory_order_relaxed);

	while(height > max_height){
		height = max_height+1;
		if(max_height_.compare_exchange_weak(max_height, height)){
			max_height = height; // EUNJI: what's this? 
			break;
		}
	}	 
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
	
			if(iterator.splice->prev_[i]->CASNext(i,iterator.splice->next_[i],nnode)){
				break; // success 
			}	
			// failure
			rv = RecomputeSpliceLevels(key, i, iterator.splice);
			assert(rv < 0);
		}
	}
	
	return true;
}

void ConcurrentSkipList::PrintStat()
{
	cout << "ConcurrentSkipList comparator count = " << cpr_cnt << endl;

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
	max_height_ = 1; 
	seq_splice = AllocateSplice(); 

    srand((unsigned)time(NULL));
	cpr_cnt = 0;
}

#if 0
ConcurrentSkipList::ConcurrentSkipList()
    :SkipList(static_cast<uint16_t>(MAX_LEVEL), 
	AllocateNode("!","!",MAX_LEVEL),1,AllocateSplice()){
    srand((unsigned)time(NULL));
	cpr_cnt = 0;

    /*for(int i=0; i<kMaxHeight_; i++){
	head_->SetNext(i, nullptr);
    }*/
}
#endif


