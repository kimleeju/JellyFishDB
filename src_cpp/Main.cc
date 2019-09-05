#define DEBUG

#include "SkipList.h"
#include "BlockedSkipList.h"
#include "ConcurrentSkipList.h"
#include "JellyFishSkipList.h"
//#include "SimpleSkipList.h"
#include "Iterator.h"
#include "BenchManager.h"

int main(int argc, char* argv[])
{ 
    SkipList* sl;

    if (argc < 4){
        cout << "Usage: ./Run ConcurrentSkipList thread_count path" << endl;
        cout << "Options: BlockedSkipList ConcurrentSkipList JellyFishSkipList" << endl;
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
	
   else if(type == "JellyFishSkipList"){
	sl = new JellyFishSkipList;
     } 
  // else if(type == "SimpleSkiplist"){
//	sl = new SimpleSkipList;
 //   }
    BenchManager bm(thread_num, path, sl);


  // bm.manage_wl();

//   if(bm.run() == 1)
//	cout<<"failed to check time"<<endl;
	cout<<bm.run()<<endl;

    return 0;
}




 
