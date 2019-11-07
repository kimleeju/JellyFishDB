#include "SkipList.h"
#include "BlockedSkipList/BlockedSkipList.h"
#include "CVSkipList/CVSkipList.h"
#include "ConcurrentSkipList/ConcurrentSkipList.h"
//#include "StrideSkipList/StrideSkipList.h"
//#include "JDKSkipList/JDKSkipList.h"
#include "JellyFishSkipList/JellyFishSkipList.h"
//#include "SimpleSkipList/SimpleSkipList.h"
//#include "Iterator.h"
#include "Bench/BenchManager.h"

using namespace std;
#if 1

int main(int argc, char* argv[])
{ 	
	SkipList* sl;

	if (argc < 5){
		cout << "Usage: ./Run Options thread_count load_trc run_trc" << endl;
		cout << "Options: BlockedSpinSkipList BlockedCVSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList" << endl;
		return -1;
	}

	string type = argv[1];
	int thread_num = atoi(argv[2]);
	char *load_trc_fname = argv[3];
	char *run_trc_fname = argv[4];

	int rv;

#if 1

	if(type == "BlockedSpinSkipList"){
		sl = new BlockedSkipList;
	}
	
	else if(type == "BlockedCVSkipList"){
		sl = new CVSkipList;
	}
#if 0
	else if(type == "JDKSkipList"){
		sl = new JDKSkipList(thread_num);
	}
	
	else if(type =="SimpleSkipList"){
		sl = new SimpleSkipList;
	}
#endif

	else if(type == "ConcurrentSkipList"){
		sl = new ConcurrentSkipList;
	}
#if 0	
	else if(type =="StrideSkipList"){
		sl = new StrideSkipList;
	}
#endif	
	else if(type == "JellyFishSkipList"){
		sl = new JellyFishSkipList;
	}
#endif
#if 0
	if(type == "BlockedCVSkipList"){
		sl = new CVSkipList;
	}
	if(type == "BlockedSpinSkipList"){
		sl = new BlockedSkipList;
	}
	else if(type == "BlockedCVSkipList"){
		sl = new CVSkipList;
	}
	else if(type == "JDKSkipList"){
		sl = new JDKSkipList(thread_num);
	}
	else if(type == "StrideSkipList"){
		sl = new StrideSkipList;
	}

	else if(type == "JellyFishSkipList"){
		sl = new JellyFishSkipList;
	}
	else if(type == "SimpleSkipList"){
		sl = new SimpleSkipList;
	}
#endif
	else{
		cout<< "failed to run"<<endl;
		cout << "Usage: ./Run Options thread_count path" << endl;
		cout << "Options: BlockedSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList" << endl;
		return -1;
	} 
	// create a bench manager 	
	BenchManager bm(thread_num, sl, type);
	rv = bm.run_trc(load_trc_fname); 
	if (rv < 0){
		cout << "Failed to load trc" << endl;
		return 0;
	}
	rv = bm.run_trc(run_trc_fname);
	if (rv < 0) {
		cout << "Failed to load trc" << endl;
		return 0;
	}

#ifdef PRINT_STAT
	sl->PrintStat();
#endif
	return 0;
}
#endif


