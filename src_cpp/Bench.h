#ifndef BENCH_H
#define BENCH_H

#include <iostream>
#include"WorkLoad.h"
#include "Util.h"
#include "SkipList.h"
#include "SimpleSkipList.h"
#define MAXVALUE 100
using namespace std;

// for each thread of workload
struct Request {
	vector<Workloads> wl_th;
	SkipList *sl;
};

class Bench {
	public:
		void set_val(int i);
		void set_op(string o); 
		void set_rv(string r);
		void set_req(Request *r);
		int get_val();
		string get_op();
		string get_rv();
		Request *get_req();
		void printf_req();
		void *do_query_with_trace(); 

//		Bench(Request *r) : r(t_arg) {}

//		friend class Writer;
	private:
	//	SkipList *sl;
		int val;  // for random value
		string op;  // operation ("put" "get" "update" "scan")
		string get_v;  // return value of skiplist->get fuction
		multimap<string, string> mmap;  // for scan
		Request *t_arg;  // temp multiple arg
};

#endif
