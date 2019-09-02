#ifndef WRITER_H
#define WRITER_H

#include "Thread.h"
#include "Bench.h"
#include <stdio.h>
#include <cstring>

class Generator : public Thread {
	Bench *b;
	public:
	Generator() {};

	void set_bench(Bench *bt){
		b=bt;
	}
	void *entry() override {
		b->do_query_with_trace();
		return 0;
	}
	void print_bench_request(){
		Request * req = b->get_req();
		printf("----------------print bench request( tid : %u )-------------------\n", req->wl_th[0].getTid());
		for(unsigned int i=0; i<req->wl_th.size();i++){
			printf("        %d  %s  %s  %d\n", req->wl_th[i].getTid(), req->wl_th[i].getOp().c_str(), req->wl_th[i].getKey().c_str(), req->wl_th[i].getCnt());
		}
		printf("------------------------------------------------------------------\n");
	}
};

#endif
