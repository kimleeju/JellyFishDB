#ifndef BENCHMANAGER_H
#define BENCHMANAGER_H
#include <vector>

#include "../SkipList.h"
#include "../Util.h"
#include "../Time.h"
#include "../Iterator.h"
#include "Thread.h"

#include <string>
#include <iostream>
#include <fstream>
#include <ostream>

#include <stdio.h>
#include <cstring>
#include <sstream>
#define MAXVALUE 100
using namespace std;


struct Request {
	string op;
	string key;

	Request(string op, string key) : op(op), key(key) {}
};

class Worker : public Thread {

	SkipList *sl;
	vector<Request> rqs;
	int ops;
	int th;

public:
	void create(int _th) {
		_create();
		th = _th;
	}

	void join() {
		_join();
	}

	void add_rq(Request rq) { rqs.push_back(rq); }

	void reset() { 
		rqs.clear(); 
		ops = 0;
	}

	void* do_query_with_trace();
	void* entry() override {
		return do_query_with_trace();
	}

public:
	Worker(SkipList* sl) : sl(sl), ops(0) {}
	~Worker() {}
};


class BenchManager 
{
	Timer timer;

public:
	int th; 
	int tot_ops; 


	SkipList *sl;
	vector<Worker> workers;
	
	int _read_trc_file(string fname); // load req to thread vector 

	int run_trc(string fname);  // get time by using Thread
	void get_stat();

	BenchManager(int th, SkipList *sl) : 
		th(th),
		sl(sl)
	{

		for(int i = 0; i < th; i++) {
			Worker w(sl); 
			workers.push_back(w);
		}

	} 
};

#endif
