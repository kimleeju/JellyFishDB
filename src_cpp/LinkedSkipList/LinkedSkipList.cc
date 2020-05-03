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
	if(iterator.Node_Linked()->Get_vqueue() != nullptr)
		return iterator.Node_Linked() -> Get_vqueue() -> value;
	else
		return iterator.Node_Linked() -> Get_value();
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
		if(temp_->Next()==nullptr)
			return;

		if(temp_->Next()->Get_key() != str_key){
			str_key = temp_->Next()->Get_key();
			--i;
		}
       	temp_=temp_->Next();
    }
#endif 	
}


Node_Linked* LinkedSkipList::FindGreaterorEqual(const string& key){
	Node_Linked* x = head_;
	int level = max_height_ -1;
	int j = MAX_LEVEL -1;
	while(j > level){
		x = x->Get_Down();
		j--;
	}	

	Node_Linked *last_bigger = nullptr;
	Node_Linked* next;
	while(true){
		next = x->Next();
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
			x = x->Get_Down();
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
	
	assert(before != nullptr);
	int cmp = 1;
	Node_Linked* after = before->Next();
	COUNT(pointer_cnt);
	while(true){
		PUT_REFERENCE(level);	
		//if(after == NULL)
			//cout<<"aaaaaaaa-------->";
		if(after) 
			cmp = KeyIsAfterNode(key, after);	
		if(!after || cmp <= 0) {
			//while(before->Get_height() > level)
			//	before = before->Get_Down();
			//after = before->Next();
			COUNT(pointer_cnt);
			COUNT(pointer_cnt);

			*sp_prev = before;
			*sp_next = after;
			return cmp;
		}
		COUNT(pointer_cnt);
        	COUNT(pointer_cnt);
		before = after;
        	after = after->Next();
	}
}

int LinkedSkipList::RecomputeSpliceLevels(const string& key, int to_level, Splice* splice)
{
	//head
	int i = max_height_-1;
	int cmp; 
	Node_Linked* start = head_;
	int j = MAX_LEVEL-1;
	while(j > i){
		start = start->Get_Down();
		j--;
	}
	while(1){
		
		cmp = FindSpliceForLevel(key, i, &splice->prev_[i], &splice->next_[i], start);
		// continue searching 
		if(i <= to_level)
		{	
			if(cmp == 0)
				return 0;
			else
				break;
		} 
		--i;
		start = splice->prev_[i+1]->Get_Down();
	}
	PUT_LEVEL(0);

	return -1;

}
#if 0


VNode_Linked* LinkedSkipList::AllocateVNode(Iterator_Linked& iterator, const string& value){
	std::atomic<char>* arena_pointer=iterator.arena.AllocateAligned(sizeof(VNode_Linked));
	VNode_Linked* x = reinterpret_cast<VNode_Linked*>(arena_pointer);
	x->value = value;
	x->NoBarrier_SetNext(nullptr);
	return x;
}
#endif
#if 1
VNode_Linked* LinkedSkipList::AllocateVNode(Iterator_Linked& iterator, const string& value){
	VNode_Linked * x = new VNode_Linked(value);
	x->NoBarrier_SetNext(nullptr);
	return x;
}
#endif

#if 1
Node_Linked* LinkedSkipList::AllocateNode(Iterator_Linked& iterator, const string& key, const string& value, int height){
	Node_Linked* x = new Node_Linked(iterator.arena,key,value,height);
	return x;
}
#endif
#if 0
Node_Linked* LinkedSkipList::AllocateNode(Iterator_Linked& iterator, const string& key, const string& value, int height){
	//Node_Linked* x = new Node_Linked(iterator.arena,key,value,height);
	std::atomic<char>* arena_pointer = iterator.arena.AllocateAligned(sizeof(Node_Linked));
	Node_Linked* x = reinterpret_cast<Node_Linked*>(arena_pointer);	
	x->Set_key(key);
	x->Set_height(height);
	x->Set_value(value);
	x->SetNext(nullptr);	
	return x;	
}
#endif
Node_Linked* LinkedSkipList::AllocateNode(const string& key, const string& value, int height){
	Node_Linked* x[height];
	for(int i = 0; i<height; i++)
	{
		if ( i ==0 ){
			x[i] = new Node_Linked(key, value, i);
			x[i]->SetDown(nullptr);
		}
		else{
			x[i] = new Node_Linked(key, "NULL", i);
			x[i]->SetDown(x[i-1]);
		}
	//	cout<<"x["<<i<<"] = "<<x[i]<<endl;
	}
	//cout<<"-----------"<<endl;
	
	return x[height-1];
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
		
	int rv = RecomputeSpliceLevels(key, 0, iterator.splice);
	
	// Allocate a new node 
	if(rv >= 0){
	
		Node_Linked* fnode = iterator.splice->next_[0];
		VNode_Linked* nvnode = AllocateVNode(iterator,value);
		#if 0
		if(fnode->Get_key() != key){
		cout<<"fnode->Get_key() = "<<fnode->Get_key()<<endl;
		cout<<"key = "<<key<<endl;
		cout<<"rm = "<<rv<<endl;	
		}
		#endif
		assert(fnode->Get_key() ==key);
		while(1){
			VNode_Linked* vq = fnode->Get_vqueue();
			
			if(vq == nullptr){
				if(fnode->CAS_vqueue(vq,nvnode)){
					return true;
				}
				continue;
			}
			nvnode->NoBarrier_SetNext(vq->NoBarrier_Next());
			if(fnode->CAS_vqueue(vq,nvnode)){
				return true;
			}
		}
			
	}
	
	for (int i =0; i<height; i++){
		Node_Linked* nnode;
		if( i == 0)	
			nnode = AllocateNode(iterator, key, value, i);
		else
			nnode = AllocateNode(iterator, key, "NULL", i);
		while(true){
			nnode->NoBarrier_SetNext(iterator.splice->next_[i]);	
			COUNT(pointer_cnt);
			if(iterator.splice->prev_[i]->CASNext(iterator.splice->next_[i],nnode)){
				COUNT(CAS_cnt);
				if(i == 0)
					nnode->SetDown(nullptr);
				else{
					nnode->SetDown(iterator.splice->prev_[i-1]->Next());
				}
				break; // success 
			}	
			// failure
			COUNT(CAS_failure_cnt);
			rv = RecomputeSpliceLevels(key, i, iterator.splice);
		}	
	}

		
	#if 0
	Node_Linked* temp =head_;
	for ( int i =0 ;i <11 ; i++)
		temp = temp->Get_Down();
	cout<<endl;
	while (temp != NULL)
	{
		cout<<temp->Get_key()<<"->";
		temp = temp->Next();
	}
#endif
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



