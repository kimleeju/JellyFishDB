#define DEBUG

#include "SkipList.h"
#include "BlockedSkipList/BlockedSkipList.h"
#include "ConcurrentSkipList/ConcurrentSkipList.h"
#include "JDKSkipList/JDKSkipList.h"
#include "JellyFishSkipList/JellyFishSkipList.h"
//#include "SimpleSkipList.h"
//#include "Iterator.h"
#include "Bench/BenchManager.h"

using namespace std;

int main(int argc, char* argv[])
{ 	
    SkipList* sl;
    if (argc < 4){
        cout << "Usage: ./Run Options thread_count path" << endl;
        cout << "Options: BlockedSkipList ConcurrentSkipList JDKSkipList JellyFishSkipList" << endl;
        return -1;
    }

    string type = argv[1];
    int thread_num = atoi(argv[2]);

    char *path = argv[3];
    cout << "type: " << type << endl;
    if(type == "BlockedSkipList"){
	sl = new BlockedSkipList;
     }

   else if(type == "ConcurrentSkipList"){
	sl = new ConcurrentSkipList;
     }
	
   else if(type == "JDKSkipList"){
	sl = new JDKSkipList;
	}
	
   else if(type == "JellyFishSkipList"){
	sl = new JellyFishSkipList;
     } 
    BenchManager bm(thread_num, path, sl);



   if(bm.run() == 1)
	cout<<"IOPS = "<<bm.run()<<endl;

    return 0;
}




 
