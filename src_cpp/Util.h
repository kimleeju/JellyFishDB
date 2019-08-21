// #define DEBUG 1 
// #define debug_print(fmt, …) \
//             do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)
#include<mutex>
#include<pthread.h>

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
    long sequence; 
    Mutex mlock;

public:
    long get_and_inc(){
        mlock.lock();
        long t = sequence; 
        sequence++;
        mlock.unlock();
        return t;
    }

    TimeStamp(long s) : sequence (s) {}
    ~TimeStamp(){}

};
