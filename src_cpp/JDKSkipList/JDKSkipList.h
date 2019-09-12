#ifndef JNI_SKIPLIST_H
#define JNI_SKIPLIST_H

#include <list>
#include <cstring>
#include <jni.h>
#include "../SkipList.h"
#include "../Iterator.h"


 

using namespace std;


class JDKSkipList : public SkipList{

public:
	JavaVM *vm;
	JNIEnv *env;
	JavaVMInitArgs vm_args;
	jmethodID mid;
	jclass jcls;
	jobjectArray jarr;
		
	jmethodID put_mid;
	jmethodID get_mid;
	JNIEnv** t_env;

public:
    int Put(string key, string value, Iterator iterator);
    string Get(string key, Iterator iterator); 
//    int remove(string key); 
	virtual void GetEnv(int t_num);
   
public:
    void RangeQuery(string start_key, int count, Iterator iterator);
    Splice* AllocateSplice();
    Node* FindLast();
    Node* FindLessThan(string key, Node** prev);
    Node* FindGreaterorEqual(string key);
    int RecomputeSpliceLevels(string key, int level, Splice* splice = 0);
    void FindSpliceForLevel(string key, int level, Node** sp_prev, Node** sp_next, Node* before);
    bool KeyIsAfterNode(string key, Node* n);
    Node* AllocateNode(string key, string value, int height); 
    int RandomHeight();
    bool Insert(string key, string value, Iterator iterator);
public:
    JDKSkipList();
	JDKSkipList(int t_num);
    ~JDKSkipList(){}
};

#endif
