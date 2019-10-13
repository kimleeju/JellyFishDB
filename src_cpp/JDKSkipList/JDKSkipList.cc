#include "JDKSkipList.h"

void JDKSkipList::SetThreadNum(int t_num){
	DEBUG(__func__ << " JDKSkipList " << t_num);

	int rv = vm->GetEnv((void **)&t_env[t_num], JNI_VERSION_1_2);

	if(rv != JNI_EDETACHED){
		DEBUG("[1] Failed to Attach Thread: Already attached or  " << t_num << " is not available");	
		return; 
	}

	// Try to attach a thread 
	rv = vm->AttachCurrentThread((void **) &t_env[t_num], NULL); 

	if(rv != 0){
		DEBUG("[2] Failed to Attach Thread = " << t_num);	
		return; 
	}

	DEBUG("Attached Thread = " << t_num); 
};

int JDKSkipList::Put(string key, string value, Iterator iterator){
#ifdef OP_EXEC
 //   t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
    iterator.Put(key,value, iterator);
 //   t_global_committed.mlock.unlock();
#endif
    return 0;
}


string JDKSkipList::Get(string key , Iterator iterator){
//    t_global_committed.mlock.lock();
	string value("deadbeaf");
#ifdef OP_EXEC
	int seq = iterator.seq;
    t_global_committed.get_and_inc();

	jstring jstr = t_env[seq]->NewStringUTF(key.c_str());	
	jobject jrslt = t_env[seq]->CallStaticObjectMethod(jcls, get_mid, jstr);
	if (!jrslt) {
		cout<<"Error : failed to get value for key "<<key<<endl;
		//return 0;
	}
	const char* get_val = t_env[seq]->GetStringUTFChars((jstring) jrslt, NULL);
	value = get_val;
#endif
	return value;
}
    


void JDKSkipList::RangeQuery(string start_key, int count, Iterator iterator ){
   /* t_global_committed.mlock.lock();
    t_global_committed.get_and_inc();
    cout<<"-----------------------------"<<endl;
    iterator.Seek(start_key);
    Node* temp_ = iterator.Node();
      for(int i=count; i > 0; --i) {
        cout<<"str_key, str_value = "<<temp_->Get_key()<<", "<<temp_->Get_value()<<endl;
	if(temp_->Next(0)!=nullptr)
       	    temp_=temp_->Next(0);
    } 
    t_global_committed.mlock.unlock();*/
}



JDKSkipList::Splice* JDKSkipList::AllocateSplice(){
    /*size_t array_size = sizeof(Node*) * (kMaxHeight_ + 1) + sizeof(Node) ;
    char* raw = new char[sizeof(Splice) + array_size*2];
    Splice* splice = reinterpret_cast<Splice*>(raw);
    splice->height_ = 0;
    splice->prev_ = reinterpret_cast<Node**>(raw + sizeof(Splice) );
    splice->next_ = reinterpret_cast<Node**>(raw + sizeof(Splice) + array_size);
    */
	/*Splice* splice = new Splice();
	splice->height_ = 0;
	splice->prev_ = new Node *[MAX_LEVEL];
	splice->next_ = new Node *[MAX_LEVEL];
    return splice;*/
}


Node* JDKSkipList::FindLast(){
    /*Node* x = head_;
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
    }*/
}

Node* JDKSkipList::FindLessThan(string key, Node** prev){
    /*int level = kMaxHeight_ -1 ;
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
    }*/
}

Node* JDKSkipList::FindGreaterorEqual(string key){
    /*Node* x = head_;
    int level = kMaxHeight_ -1;
    Node *last_bigger = nullptr;
    while(true){
        Node* next = x->Next(level);
        int cmp = (next == nullptr || next == last_bigger) ? 1 : next->Get_key().compare(key);

        if(cmp >= 0 &&level ==0){
            return next;
        }
        else if (cmp < 0){
//	 if(next->Next(level) !=nullptr)	
            x= next;
//	 else
//	    return nullptr;
        }
        else{
            last_bigger = next;
            level --;
        }
    }*/
}
    


int JDKSkipList::RecomputeSpliceLevels(string key, int level, int low,Splice* splice){
   /* Node* before = head_;
    for(int i =level -1  ;i>=0; --i){
        FindSpliceForLevel(key, i, &seq_splice->prev_[i], &seq_splice->next_[i],before);
    }*/
	return 0;
}


void JDKSkipList::FindSpliceForLevel(string key, int level, int cur_level, Node** sp_prev, Node** sp_next, Node* before){
/*Node* after = before ->Next(level);
    while(true){
        if(!KeyIsAfterNode(key, after)){
            *sp_prev = before;
            *sp_next = after;
            return;
        }
        before = after;
        if(after != nullptr)
            after = after->Next(level);
    }*/
}

bool JDKSkipList::KeyIsAfterNode(string key, Node* n){
  return (n != nullptr) && (key.compare(n->Get_key()) > 0);
} 




Node* JDKSkipList::AllocateNode(string key, string value, int height){
   //auto prefix = sizeof(atomic<Node*>) * (height-1);
   //cout<<"prefix = "<<prefix<<endl;
   //char* raw = new char [prefix +sizeof(Node)];
   //cout<<"raw = "<<raw<<endl;
   //printf("raw = %p\n",raw);
   
  //Node* x = reinterpret_cast<Node*>(raw + prefix);   
  //cout<<"x->Get_eky() = "<<x->Get_key()<<endl;
   Node* x = new Node(key,value,height);
  // for(int i=0;i<height;i ++){
  //     x->SetNext(i,nullptr);
       //assert(x->Next(i));
  // }
//cout<<"str_key = "<<x->Get_key()<<endl;
//cout<<"UnstashHeight() = "<<x->UnstashHeight()<<endl;
//cout<<"str_key = "<<x->Get_key().capacity()<<endl;
//cout<<"str_key.capacity() = "<<x->str_key.capacity()<<endl;
//cout<<"sizeof(string) = "<<sizeof(string)<<endl;
//cout<<"sizeof(key) = " <<sizeof(key)<<endl;   
//cout<<"key.capacity() = "<<key.capacity()<<endl;
 
//   x->Set_key(key);
//   x->StashHeight(height);
//   x->Set_key(key);
//   x->Set_value(value);
   return x;
} 

int JDKSkipList::RandomHeight(){
   int height, balancing, pivot;
   balancing =2 ;
   height = 1;
   pivot = 1000/balancing;
   while(height < kMaxHeight_ && height < pivot && (rand()%1000)<pivot){
    height++;
   }
   return height;
}



bool JDKSkipList::Insert(string key, string value, Iterator iterator)
{
	int seq = iterator.seq;
	// put <k, v>
	jobjectArray jarr_temp = (jobjectArray)t_env[seq]->NewObjectArray(2, 
							t_env[seq]->FindClass("java/lang/String"),
							t_env[seq]->NewStringUTF("str")
							);
	
	
	t_env[seq]->SetObjectArrayElement(jarr_temp, 0, t_env[seq]->NewStringUTF(key.c_str()));
	t_env[seq]->SetObjectArrayElement(jarr_temp, 1, t_env[seq]->NewStringUTF(value.c_str()));
	t_env[seq]->CallStaticVoidMethod(jcls, put_mid, jarr_temp);	

	return true;
}

JDKSkipList::JDKSkipList(int t_num) : SkipList(
			static_cast<uint16_t>(MAX_LEVEL), 
			AllocateNode("!","!",MAX_LEVEL),1,AllocateSplice())
{
	DEBUG(__func__ << " " << t_num );
	srand((unsigned)time(NULL));

	// contruct jvm 
	vm_args.version = JNI_VERSION_1_2;
	vm_args.nOptions = 0;

	jint res = JNI_CreateJavaVM(&vm, (void **)&env, &vm_args);
	if (res < 0) {
		cout << "Error: Failed to create jvm" << endl;
		return; 
	}

	jcls = env->FindClass("MyConcurrentSkipListMap");
	if (!jcls) {
		cout << "Error: Unable to find MyConcurrentSkipListMap" << endl;
		return; 
	}

	// create skip list
	create_mid = env->GetStaticMethodID(jcls, "create_sl", "()V");
	if (!create_mid){
		cout << "Error: Unable to find method: create_sl" << endl;
		return; 
	}

	env->CallStaticVoidMethod(jcls, create_mid);

	// mapping functions 
	put_mid = env->GetStaticMethodID(jcls, "Put", "([Ljava/lang/String;)V");
	if(!put_mid){
		cout << "Error: Unable to find method: Put" << endl;
		return;
	}

	get_mid = env->GetStaticMethodID(jcls, "Get", "(Ljava/lang/String;)Ljava/lang/String;");
	if(!get_mid){
		cout << "Error: Unable to find method: Get" << endl;
		return;
	}

	// create JNI Environment size of threads 	
	t_env = new JNIEnv*[t_num];	
	if(!t_env) {
		cout << "Error: Failed to create JNIEnv" << endl;
		return;
	}

	return;
}


