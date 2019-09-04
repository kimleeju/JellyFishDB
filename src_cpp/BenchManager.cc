#include "BenchManager.h"
#include <iostream>
#include <stdio.h>

using namespace std;
BenchManager::BenchManager(int t, char *p, SkipList *t_s) : th_num(t), path(p), sl(t_s) {
	w_vec.resize(t);
	gnrtor = new Generator[t];
	req = (Request *)malloc(sizeof(Request) * t);
	req->sl = sl;
	for(int i = 0; i < t ; i++){
		req[i].sl = sl;
	}
}

void BenchManager::prepare(){
	//	r->sl = s;
}/*
void BenchManager::print_vector(){
	r->v.assign(v[0].begin(), v[0].end());
	cout<<"workload[0] : 의 size"<<v[0].size()<<endl;
	for (unsigned int i = 0; i < v[0].size(); i++) {
	//	cout << "workload[" << i << "] 의 size  "<<v[i].size() << endl;
		//for (unsigned int j = 0; j < v[i].size(); j++) {
			cout << v[0][i].getTid() << " " << v[0][i].getOperation() << " " << v[0][i].getKey() << " " << v[0][i].getCount() << endl;
	//	}
	}
}*/
int BenchManager::run(){
	class Time time;
	// time start
	if(gettimeofday(&time.get_start(), NULL)==-1){
		cout<<"failed to read time"<<endl;
		return 1;
	}
	// set benmark for each thread
	for(int i=0;i<th_num;i++){
		Bench *bnch = new Bench;
		req[i].wl_th = w_vec[i];
//		req[i].wl_th.assign(w_vec[i].begin(), w_vec[i].end());	
 		bnch->set_req(&req[i]);  // save request each Bench class
		gnrtor[i].set_bench(bnch);
	}
	// create thread
	for(int i=0;i<th_num; i++){
		gnrtor[i].create();
	}
	// join thread
	for(int i=0; i<th_num; i++){
		gnrtor[i].join();
	}

	// time end
	if(gettimeofday(&time.get_end(), NULL)==-1){
		cout<<"failed to read time"<<endl;
		return 1;
	}

	time.print_result();

	return 0;
}
void BenchManager::manage_wl(){
	wl.save_workloads(w_vec, path);
}
void BenchManager::get_stat(){
}
