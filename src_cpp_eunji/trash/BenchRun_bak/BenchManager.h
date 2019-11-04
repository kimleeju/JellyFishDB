#ifndef BENCHMANAGER_H
#define BENCHMANAGER_H
#include "SkipList.h"
#include "Bench.h"
#include "WorkLoad.h"
#include "Util.h"
#include "Generator.h"
#include <vector>
#include "Time.h"


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
