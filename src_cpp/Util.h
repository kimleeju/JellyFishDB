// #define DEBUG 1 

//             do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#ifndef UTIL_H
#define UTIL_H
#include<mutex>
#include<pthread.h>
#include<atomic>

class Mutex {

    pthread_mutex_t _lock;

public:
    void lock() {
        pthread_mutex_lock(&_lock);
    }
    void unlock() {
        pthread_mutex_unlock(&_lock);
    }

    Mutex() { 
        //_lock = PTHREAD_MUTEX_INITIALIZER; 
    }
    ~Mutex(){}
};

class TimeStamp {
   atomic<long> sequence; 
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
