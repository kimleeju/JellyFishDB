#define DEBUG

#include "SkipList.h"
//#include "BlockedSkipList.h"
//#include "ConcurrentSkipList.h"
//#include "JellyFishSkipList.h"
#include "SimpleSkipList.h"


int main(int argc, char* argv[])
{ 
    SkipList* sl;

    if (argc < 2){
        cout << "Usage: ./Run ConcurrentSkipList" << endl;
        cout << "Options: BlockedSkipList ConcurrentSkipList JellyFishSkipList" << endl;
        return -1;
    }

    string type(argv[1]);
    cout << "type: " << type << endl;
#if 0    
    if (type == "BlockedSkipList"){
        sl = new BlockedSkipList();

    } else if (type == "ConcurrentSkipList"){
        sl = new ConcurrentSkipList();

    } else if (type == "JellyFishSkipList"){
        sl = new JellyFishSkipList();
    }
#endif
	sl = new SimpleSkipList();


    return 0;
}




 
