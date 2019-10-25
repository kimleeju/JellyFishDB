#include "BlockedSkipList.h"

void BlockedSkipList::GetEnv(int t_num){
	return;
}

int BlockedSkipList::Put(string key, string value, Iterator iterator){
    t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
    iterator.Put(key,value, iterator);
    t_global_committed.mlock.unlock();
    return 0;
}


string BlockedSkipList::Get(string key, Iterator iterator){
//    t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
    iterator.Seek(key);
    string get_value = iterator.Node()->Get_value();
//    t_global_committed.mlock.unlock();
    return get_value;
}
    


void BlockedSkipList::RangeQuery(string start_key, int count, Iterator iterator ){
//    t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
//    cout<<"-----------------------------"<<endl;
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
      //for(int i=count; i > 0; --i) {
    int i =count;  
	 while(i>0){
		// cout<<"str_key, str_value = "<<temp_->Get_key()<<", "<<temp_->Get_value()<<endl;
		if(temp_->Next(0)==nullptr)
			return;
		if(temp_->Next(0)->Get_key() != temp_->Get_key())
			--i;
       	temp_=temp_->Next(0);
    } 
//    t_global_committed.mlock.unlock();
}



BlockedSkipList::Splice* BlockedSkipList::AllocateSplice(){
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


Node* BlockedSkipList::FindLast(){
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

Node* BlockedSkipList::FindLessThan(const string& key, Node** prev){
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

Node* BlockedSkipList::FindGreaterorEqual(const string& key){
    Node* x = head_;
    int level = kMaxHeight_ -1;
    Node *last_bigger = nullptr;
    while(true){
        Node* next = x->Next(level);
        int cmp = (next == nullptr || next == last_bigger) ? 1 : Comparator(next->Get_key(),key);

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
    


int BlockedSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice){

	// head 
	int i = MAX_LEVEL-1;
	FindSpliceForLevel(key, i, &seq_splice->prev_[i], &seq_splice->next_[i], head_);
	
	while(i > to_level) {
		--i;
		FindSpliceForLevel(key, i, &seq_splice->prev_[i], &seq_splice->next_[i], seq_splice->prev_[i+1]);
	}
	return -1;

}



void BlockedSkipList::FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before){
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

int  BlockedSkipList::Comparator(string key1,string key2){
	return key1.compare(key2);
}




Node* BlockedSkipList::AllocateNode(const string& key, const string& value, int height){
   Node* x = new Node(key,value,height);
   return x;
} 

bool BlockedSkipList::KeyIsAfterNode(const string& key, Node* n){
	if(n == nullptr)
		return false;
	//cpr_cnt++;
	return key.compare(n->Get_key()) > 0;
}

#if 0
int BlockedSkipList::RandomHeight()
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
#endif




bool BlockedSkipList::Insert(string key, string value, Iterator iterator){

 // Node* nnode = AllocateNode(key, value, RandomHeight());
  int height = RandomHeight();
  int max_height = max_height_.load(std::memory_order_relaxed);
    if(height > max_height){
		max_height_ = height;
		max_height = height;   
    }
    int rv = RecomputeSpliceLevels(key, 0);
	Node* nnode = AllocateNode(key, value, height);
     for(int i=0;i<height;++i){  
        nnode->SetNext(i, seq_splice->next_[i]);
        seq_splice->prev_[i]->SetNext(i,nnode);

      }
#if 0
	// debug
    iterator.Seek(key);
    string get_value = iterator.Node()->Get_value();

	if(get_value.compare(value) != 0)
		cout << "insert: " << value << " returned " << get_value << endl;
#endif	
	return true;

}

void BlockedSkipList::PrintStat()
{
	cout << "BlockedSkipList comparator count = " << cpr_cnt << endl;

}
void BlockedSkipList::ResetStat()
{
	cpr_cnt = 0;
}

BlockedSkipList::BlockedSkipList()
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

