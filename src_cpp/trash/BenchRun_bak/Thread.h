#ifndef THREAD_H
#define THREAD_H

#include <iostream>
#include <pthread.h>
//#include <Windows.h>
using namespace std;

// 라이브러리 내부 클래스
class Thread{
	public:
		pthread_t tid;
		pid_t pid;
	private:
		static void *_entry_func(void* arg);

	protected:
		virtual void *entry() = 0;
	public:
		Thread() : tid(0), pid(0){};
		~Thread(){};
		const pthread_t &get_tid() const {return tid;}
		pid_t get_pid() const{return pid;}
		void join();
		void create();
};

#endif
