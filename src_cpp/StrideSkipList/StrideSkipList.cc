#include  "StrideSkipList.h"

int StrideSkipList::Put(string key, string value, Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Put(key,value, iterator);
    return 0;
}


string StrideSkipList::Get(string key, Iterator iterator){
    t_global_committed.get_and_inc();
    iterator.Seek(key);
    string get_value = iterator.Node()->Get_value();
    return get_value;
}



void StrideSkipList::RangeQuery(string start_key, int count, Iterator iterator){
    t_global_committed.get_and_inc();
//    cout<<"-----------------------------"<<endl;
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
    int i = count;
	while(i>1){  
		if(temp_->Get_stride_next()==nullptr){
			if(temp_->Next(0)==nullptr);
				return;
			temp_=temp_->Next(0);
			--i;
		}
		else{
//			cout<<"aaaaaa"<<endl;
       		temp_=temp_->Get_stride_next();
			--i;
    	}
	} 	
}

Node* StrideSkipList::FindLast(){
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

Node* StrideSkipList::FindLessThan(const string& key, Node** prev){
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

Node* StrideSkipList::FindGreaterorEqual(const string& key){
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
 


StrideSkipList::Splice* StrideSkipList::AllocateSplice(){
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

int StrideSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice){
   
	// head 
	int i = MAX_LEVEL-1;
	FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], head_);

	while(i > to_level) {
		--i;
		FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], splice->prev_[i+1]);
	}
	return -1;
}


void StrideSkipList::FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before){
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


bool StrideSkipList::KeyIsAfterNode(const string& key, Node* n){
	if(n == nullptr)
		return false;

	//cpr_cnt++;
	return key.compare(n->Get_key()) > 0;
}


Node* StrideSkipList::AllocateNode(const string& key, const string& value, int height){
   Node* x = new Node(key,value, height);
   return x;
}
int StrideSkipList::Comparator(string key1, string key2){
	return key1.compare(key2);
}

bool StrideSkipList::Insert(string key, string value, Iterator iterator){
  //Node* nnode = AllocateNode(key, value, RandomHeight());
//  Splice* splice = AllocateSplice();
	int height = RandomHeight();  
	Node* nnode = AllocateNode(key, value, height);

	int rv = RecomputeSpliceLevels(nnode->Get_key(), 0,iterator.splice);
    
	for(int i=0; i<height ; i++){
		while(true){
			nnode->NoBarrier_SetNext(i, iterator.splice->next_[i]);
			if(iterator.splice->prev_[i]->CASNext(i,iterator.splice->next_[i],nnode)){
				//success
				break;
			}
			
			rv = RecomputeSpliceLevels(nnode->Get_key(), i,iterator.splice);
		}
	}
	
	if(iterator.splice->next_[0] != nullptr && iterator.splice->next_[0]->Get_key() == key)
	{	
		nnode->Set_stride_next(iterator.splice->next_[0]);
	}
   return true;
     
}

void StrideSkipList::PrintStat()
{
	cout << "StrideSkipList comparator count = " << cpr_cnt << endl;

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
