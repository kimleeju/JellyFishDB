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
#define MAXVALUE 100
using namespace std;


class Workloads {
	public:
		void setTid(int t);
		void setKey(string k);
		void setOp(string o);
		void setCnt(int c);
		int getTid();
		string getKey();
		string getOp();
		int getCnt();

		void save_workloads(vector<vector<Workloads> > &v, char *p);
	private:
		int tid;
		string key;
		string op;
		int cnt;
};


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
		void *do_query_with_trace(int seq); 

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

class Generator : public Thread {
	Bench *b;
	public:
	int seq;
	Generator() {};

	void set_bench(Bench *bt){
		b=bt;
	}
	void *entry() override {
		b->do_query_with_trace(seq);
		return 0;
	}
	void print_bench_request(){
		Request * req = b->get_req();
		printf("----------------print bench request( tid : %u )-------------------\n", req->wl_th[0].getTid());
		for(unsigned int i=0; i<req->wl_th.size();i++){
			printf("        %d  %s  %s  %d\n", req->wl_th[i].getTid(), 
					req->wl_th[i].getOp().c_str(), req->wl_th[i].getKey().c_str(), req->wl_th[i].getCnt());
		}
		printf("------------------------------------------------------------------\n");
	}
};



class BenchManager 
{
 	public:
	void prepare();
	unsigned long run();  // get time by using Thread
	void get_stat();
//	void print_vector();

	BenchManager(int t, char *p, SkipList *t_s);

	private:
	int th_num;  // thread num
	char *path;  // trace path
	SkipList *sl;
	vector<vector<Workloads> > w_vec;
	Generator *gnrtor;
//	Bench b;
	Request *req;
	Workloads wl;
};

#endif
