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
//    t_global_committed.mlock.unlock();
    return iterator.Node()->Get_value();
}
    


void BlockedSkipList::RangeQuery(string start_key, int count, Iterator iterator ){
	t_global_committed.get_and_inc();
	iterator.Seek(start_key);
	Node* temp_ = iterator.Node();
	int i = count;
	
	if(temp_==nullptr)
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


Node* BlockedSkipList::FindGreaterorEqual(const string& key){
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
    


int BlockedSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice){

	// head 
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



int BlockedSkipList::FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before){

	//assert(before != nullptr);
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

#if 0
int  BlockedSkipList::Comparator(string& key1,string key2){
	return key1.compare(key2);
}
#endif




Node* BlockedSkipList::AllocateNode(const string& key, const string& value, int height){
   Node* x = new Node(key,value,height);
   return x;
} 

int BlockedSkipList::KeyIsAfterNode(const string& key, Node* n){
	if(n == nullptr)
		return -1;
	//cpr_cnt++;
	return key.compare(n->Get_key());
}


bool BlockedSkipList::Insert(string key, string value, Iterator iterator){

 // Node* nnode = AllocateNode(key, value, RandomHeight());
	int height = RandomHeight();
    int rv = RecomputeSpliceLevels(key, 0, iterator.splice);
	Node* nnode = AllocateNode(key, value, height);
     for(int i=0;i<height;++i){  
        nnode->SetNext(i, iterator.splice->next_[i]);
       	iterator.splice->prev_[i]->SetNext(i,nnode);
     
		COUNT(pointer_cnt);
		COUNT(pointer_cnt);
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
	cout << "BlockedSpinSkipList comparator count = " << cnt << endl;
	cout << "BlockedSpinSkipList CAS count = "<< CAS_cnt << endl;
	cout << "BlockedSpinSkipList pointer update count = " << pointer_cnt << endl;
	cout << "BlockedSpinSkipList CAS failure count = " << CAS_failure_cnt << endl;
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
	seq_splice = AllocateSplice(); 

    srand((unsigned)time(NULL));
	cpr_cnt = 0;
}

