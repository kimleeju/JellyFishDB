#include "Thread.h"
#include <assert.h>
#include <stdio.h>



void *Thread::_entry_func(void *arg) {
	((Thread *)arg)->entry();
	return NULL;
}

//const pthread_t &Thread::get_tid() const {
//	return tid;
//}

void Thread::join() {
	int status = pthread_join(tid, NULL);
	if(status==0){
//		printf("success thread join\n");
	}
	else{
//		printf("fail thread join\n");
	}
}

void Thread::create() {
	int r = pthread_create(&tid, NULL, _entry_func, (void *)this);
	if(r==0){
//		printf("success thread create\n");	
	}
	else{
//		printf("fail thread create\n");
	}
}
