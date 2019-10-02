#define DEBUG

#include "SkipList.h"
#include "BlockedSkipList/BlockedSkipList.h"
#include "CVSkipList/CVSkipList.h"
#include "ConcurrentSkipList/ConcurrentSkipList.h"
#include "StrideSkipList/StrideSkipList.h"
#include "JDKSkipList/JDKSkipList.h"
#include "JellyFishSkipList/JellyFishSkipList.h"
#include "SimpleSkipList/SimpleSkipList.h"
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

	char *l_path = argv[3];
	char *r_path = argv[4];
#if 0
	// cout << "type: " << type << endl;:w
	if(type == "BlockedCVSkipList"){
		sl = new CVSkipList;
	}
#else
	if(type == "BlockedSpinSkipList"){
		sl = new BlockedSkipList;
	}
	else if(type == "BlockedCVSkipList"){
		sl = new CVSkipList;
	}
	else if(type == "ConcurrentSkipList"){
		sl = new ConcurrentSkipList;
	}
	else if(type == "StrideSkipList"){
		sl = new StrideSkipList;
	}
	else if(type == "JDKSkipList"){
		sl = new JDKSkipList(thread_num);
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
	BenchManager bm(thread_num, l_path, r_path, sl);

	//bm.manage_wl();
	//	cout<<"IOPS = "<<bm.run()<<endl;
#if 0
	=======

		/*
		   >>>>>>> 2f5d29caa9c1f9197581464219767605c356331c
		   if(bm.run() == 1)
		   cout<<"failed to check time"<<endl;
		 */
#endif
	bm.load_trc(); 
	cout<<"IOPS = "<<bm.run_trc()<<endl;	
	cout<<", cnt = "<<sl->cnt<<endl;
	//	cout<<bm.run_trc()<<endl;

	return 0;
}




#endif
#if 0

int main(){
	SkipList *sl;
	Iterator* it;
	int seq;
	sl = new ConcurrentSkipList;
	it = new Iterator(sl,seq);
	sl->Put("aaaaaaaa", "111", *it);

	sl->Put("aaaaaaaa", "222", *it);

	sl->Put("aaaaaaaa", "333", *it);

	sl->Put("aaaaaaaa", "444", *it);

	sl->Put("aaaaaaaa", "555", *it);

	sl->Put("aaaaaaaa", "666", *it);

	cout<<sl->Get("aaaaaaaa", *it)<<endl;
}
#endif
