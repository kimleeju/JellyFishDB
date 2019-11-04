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
    t_global_committed.get_and_inc();
    iterator.Put(key,value, iterator);
#endif
    return 0;
}


string JDKSkipList::Get(string key , Iterator iterator){
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


#if 0 
bool JDKSkipList::KeyIsAfterNode(string key, Node* n){
  return (n != nullptr) && (key.compare(n->Get_key()) > 0);
} 
#endif

Node* JDKSkipList::AllocateNode(const string& key, const string& value, int height){
   Node* x = new Node(key,value,height);
   return x;
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

void JDKSkipList::PrintStat()
{
	cout << __func__ << endl;
	env->CallStaticVoidMethod(jcls, print_mid);
}


JDKSkipList::JDKSkipList(int t_num)
{
	string key = "!";
	string val = "!";
	head_ = AllocateNode(key, val, MAX_LEVEL); 
	kMaxHeight_ = MAX_LEVEL;	
	max_height_ = 1; 
	seq_splice = AllocateSplice(); 

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


	// print skip list
	print_mid = env->GetStaticMethodID(jcls, "print_stat", "()V");
	if (!print_mid){
		cout << "Error: Unable to find method: print_stat" << endl;
		return; 
	}


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

#if 0
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


	// print skip list
	print_mid = env->GetStaticMethodID(jcls, "print_stat", "()V");
	if (!print_mid){
		cout << "Error: Unable to find method: print_stat" << endl;
		return; 
	}


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

#endif
