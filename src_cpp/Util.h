// #define DEBUG 1 

//             do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#ifndef UTIL_H
#define UTIL_H
#include<mutex>
#include<pthread.h>
#include<atomic>

class Mutex {
public:
	pthread_cond_t cond;	
    pthread_mutex_t _lock;
    void lock() {
        pthread_mutex_lock(&_lock);
    }

    void unlock() {
        pthread_mutex_unlock(&_lock);
    }
	
	void wait(){
		pthread_cond_wait(&cond,&_lock);
	}

	void signal(){
		pthread_cond_signal(&cond);
	}

    Mutex() {
			pthread_mutex_init(&_lock, NULL);
   			pthread_cond_init(&cond,NULL);
	 }
	Mutex(Mutex* mu){
		cond = mu->cond;
		_lock = mu->_lock;
	}
    ~Mutex(){}
};



class TimeStamp {
   std::atomic<long> sequence; 
    //long sequence;
  public:  
    Mutex mlock;

public:
    long get_and_inc(){
       // mlock.lock();
        long t = sequence; 
        sequence++;
       // mlock.unlock();
        return t;
    }
    TimeStamp() : sequence (0){}
    TimeStamp(long s) : sequence (s) {}
    ~TimeStamp(){}

};


#endif
