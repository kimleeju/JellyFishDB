#ifndef CV_SKIPLIST_H
#define CV_SKIPLIST_H

#include <list>
#include <cstring>
#include "../SkipList.h"
#include "../Iterator.h"

#include <assert.h>
using namespace std;
class CVSkipList : public SkipList{

public:
    int Put(string key, string value, Iterator iterator);
    string Get(string key, Iterator iterator); 
//    int remove(string key); 

   
public:
	template<typename T>
	class Queue{
			public:
				pthread_mutex_t lock;

			public:
    		void push_back( const T& value ){
				//pthread_mutex_lock(&lock);
        		m_que.push(value);
				//pthread_mutex_unlock(&lock);
    		}

    		void pop(){
        		m_que.pop();
    		}
			
			T front(){
				//pthread_mutex_lock(&lock);
				T tmp = m_que.front();
				//pthread_mutex_unlock(&lock);
				return tmp;
			}

			T back(){
//				pthread_mutex_lock(&lock);
        		T tmp = m_que.back();
//				pthread_mutex_unlock(&lock);
        		return tmp;
    		}
			
			void pop_front(){	
//				pthread_mutex_lock(&lock);
        		m_que.pop();
//				pthread_mutex_unlock(&lock);
    		}

	
			bool empty(){
//				pt
        		int result = m_que.empty();
//				pthread_mutex_unlock(&lock);
				return result;
    		}

	//		void swap(std::deque<T>& x) {
	//			m_deque.swap(x);
	//		}

			private:
    			std::queue<T> m_que;
    			//mutable std::mutex m_mutex;
	};
std::atomic<int> cnt ;
	pthread_mutex_t mu_lock;
	Queue<Node*> req_q;	
	void RangeQuery(string start_key, int count, Iterator iterator);
    Splice* AllocateSplice();
    Node* FindLast();
    Node* FindLessThan(string key, Node** prev);
    Node* FindGreaterorEqual(string key);
    int RecomputeSpliceLevels(string key, int level, int low,Splice* splice = 0);
    void FindSpliceForLevel(string key, int level, int cur_level,Node** sp_prev, Node** sp_next, Node* before);
    bool KeyIsAfterNode(string key, Node* n);
    Node* AllocateNode(string key, string value, int height); 
    int RandomHeight();
    bool Insert(string key, string value, Iterator iterator);
public:
    CVSkipList();
    ~CVSkipList(){}
};

#endif
