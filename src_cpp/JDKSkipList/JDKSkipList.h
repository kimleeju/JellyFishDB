#ifndef JNI_SKIPLIST_H
#define JNI_SKIPLIST_H

#include <list>
#include <cstring>
#include <jni.h>
#include <assert.h>
#include "../SkipList.h"
#include "../Iterator.h"


 

using namespace std;


class JDKSkipList : public SkipList{

public:
	JavaVM *vm;
	JNIEnv *env;
	JavaVMInitArgs vm_args;
	jclass jcls;
	jobjectArray jarr;
		
	jmethodID create_mid;
	jmethodID print_mid;
	jmethodID put_mid;
	jmethodID get_mid;
	JNIEnv** t_env;

public:
    int Put(string key, string value, Iterator iterator);
    string Get(string key, Iterator iterator); 
    void RangeQuery(string start_key, int count, Iterator iterator){}
	void SetThreadNum(int t_num) override;
   
public:
    Splice* AllocateSplice(){}
    Node* FindLast(){}
    Node* FindLessThan(const string& key, Node** prev){}
    Node* FindGreaterorEqual(const string& key){}
    int RecomputeSpliceLevels(const string& key, int to_level, Splice* splice = 0){}
    void FindSpliceForLevel(const string& key, int level, Node** sp_prev, Node** sp_next, Node* before){}
    bool KeyIsAfterNode(const string& key, Node* n){return true;}
    Node* AllocateNode(const string& key, const string& value, int height); 
    //Node* AllocateNode(string key, string value, int height); 
    bool Insert(string key, string value, Iterator iterator);
	void PrintStat();
public:
	JDKSkipList(int t_num);
    ~JDKSkipList(){
		PrintStat();
	}
};

#endif
