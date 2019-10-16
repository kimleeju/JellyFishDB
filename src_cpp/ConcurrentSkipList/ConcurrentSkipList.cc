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

Node* ConcurrentSkipList::FindLessThan(string key, Node** prev){
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

Node* ConcurrentSkipList::FindGreaterorEqual(string key){
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
   /* size_t array_size = sizeof(Node*) * (kMaxHeight_ + 1) + sizeof(Node) ;
    char* raw = new char[sizeof(Splice) + array_size*2];
    Splice* splice = reinterpret_cast<Splice*>(raw);
    splice->height_ = 0;
    splice->prev_ = reinterpret_cast<Node**>(raw + sizeof(Splice) );
    splice->next_ = reinterpret_cast<Node**>(raw + sizeof(Splice) + array_size);
    return splice;*/
	Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node *[MAX_LEVEL];
	splice->next_ = new Node *[MAX_LEVEL];
   return splice;
}

bool ConcurrentSkipList::KeyIsAfterNode(string key, Node* n){
	return (n != nullptr) && (key.compare(n->Get_key()) > 0);
}


void ConcurrentSkipList::FindSpliceForLevel(string key, int level, int cur_level, 
		Node** sp_prev, Node** sp_next, Node* before)
{
	assert(before != NULL);

	Node* after = before->Next(cur_level);

	while(true){
		if(!KeyIsAfterNode(key, after)){
			*sp_prev = before;
			*sp_next = after;
			return;
		}
        before = after;
        after = after->Next(cur_level);
	}
}

int ConcurrentSkipList::RecomputeSpliceLevels(string key, int level, int low, Splice* splice)
{
	for(int i = MAX_LEVEL-1; i >= low; --i){

		if(i==MAX_LEVEL-1)
        	FindSpliceForLevel(key, level, i, &splice->prev_[i], &splice->next_[i], head_);
		else
			FindSpliceForLevel(key, level, i, &splice->prev_[i], &splice->next_[i], splice->prev_[i+1]);
	 }
	return -1;
}


int ConcurrentSkipList::Comparator(string key1, string key2){
#if 0	
	if(key1.length() < key2.length())
		return  -1;
	else if ( key1.length() > key2.length())
		return 1;
  return key1.compare(key2);
#endif
	return key1.compare(key2);
}

Node* ConcurrentSkipList::AllocateNode(string key, string value, int height){
   /*auto prefix = sizeof(Node*) * (height);
   char* raw = new char [prefix  + sizeof(Node*)+sizeof(Node)];
   Node* x = reinterpret_cast<Node*>(raw + prefix);

   for(int i=0;i<height;i ++){
       x->SetNext(i,nullptr);
       //assert(x->Next(i))
   }
	
   x->StashHeight(height);
   x->Set_key(key);
   x->Set_value(value);*/
   Node* x = new Node(key,value, height);
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

#if 0
   int height, balancing, pivot;
   balancing =2 ;
   height = 1;
   pivot = 1000/balancing;

   while(height < kMaxHeight_ && height < pivot && (rand()%1000)<pivot){
    height++;
   }
   return height;
#endif
}



bool ConcurrentSkipList::Insert(string key, string value, Iterator iterator)
{
	// update current max height
	int height = RandomHeight();

#ifdef PRINT_HEIGHT
	cout << height << endl;
#endif

	int max_height = max_height_.load(std::memory_order_relaxed);

	while(height > max_height){
		if(max_height_.compare_exchange_weak(max_height, height)){
			max_height = height; // EUNJI: what's this? 
			break;
		}
	}	 

	// Allocate a new node 
	Node* nnode = AllocateNode(key, value, height);

	int rv = RecomputeSpliceLevels(nnode->Get_key(), nnode->Get_height(),0,iterator.splice);
	assert(rv < 0);

	for(int i=0; i < height; i++){
		while(true){
			nnode->NoBarrier_SetNext(i, iterator.splice->next_[i]);
	
			if(iterator.splice->prev_[i]->CASNext(i,iterator.splice->next_[i],nnode)){
				break; // success 
			}	
			// failure
			rv = RecomputeSpliceLevels(nnode->Get_key(), nnode->Get_height(),i,iterator.splice);
			assert(rv < 0);
		}
	}
	
	return true;
}

ConcurrentSkipList::ConcurrentSkipList()
    :SkipList(static_cast<uint16_t>(MAX_LEVEL), AllocateNode("!","!",MAX_LEVEL),1,AllocateSplice()){
    srand((unsigned)time(NULL));

    /*for(int i=0; i<kMaxHeight_; i++){
	head_->SetNext(i, nullptr);
    }*/
}



