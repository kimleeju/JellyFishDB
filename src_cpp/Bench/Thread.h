#ifndef THREAD_H
#define THREAD_H

#include <iostream>
#include <pthread.h>
#include <assert.h>
#include <stdio.h>

//#include <Windows.h>
using namespace std;

// 라이브러리 내부 클래스
class Thread{
	public:
		pthread_t tid;
		pid_t pid;
	private:
		static void *_entry_func(void* arg) {
			assert(arg);
			((Thread *)arg)->entry();
			return NULL;
		}

	protected:
		virtual void *entry() = 0;

		void _join() {
			int rv = pthread_join(tid, NULL);
			assert(rv == 0);
		}
		
		
		void _create(){
			int rv = pthread_create(&tid, NULL, _entry_func, (void *)this);
			assert(rv == 0);
		}


		Thread() : tid(0), pid(0){};
		~Thread(){};
};

#endif
