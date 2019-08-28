#define DEBUG

#include "SkipList.h"
#include "BlockedSkipList.h"
#include "ConcurrentSkipList.h"
#include "JellyFishSkipList.h"
//#include "SimpleSkipList.h"
#include "Iterator.h"


int main(int argc, char* argv[])
{ 
    SkipList* sl;

    if (argc < 4){
        cout << "Usage: ./Run ConcurrentSkipList max_level node_count" << endl;
        cout << "Options: BlockedSkipList ConcurrentSkipList JellyFishSkipList" << endl;
        return -1;
    }

    string type = argv[1];
    int max_level = atoi(argv[2]);
    int node_count = atoi(argv[3]);
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
    if(type == "BlockedSkipList"){ 
        sl = new BlockedSkipList(max_level, node_count);
        for (int i=0;i<node_count; i++){
       cout<<"-----------------------------"<<endl;
	sl->put(sl->randomString(),sl->randomString());
        }
	cout<<"-----------------------------"<<endl; 
	sl->RangeQuery("0", node_count);
	cout<<"-----------------------------"<<endl;
	if(sl->get_impl("a") != nullptr)
	  cout<<sl->get("a")<<endl;
	else
	  cout<<"nullptr"<<endl;    
    }
	
   else if(type == "ConcurrentSkipList"){ 
        sl = new ConcurrentSkipList(max_level, node_count);
        for (int i=0;i<node_count; i++){
       cout<<"-----------------------------"<<endl;
	sl->put(sl->randomString(),sl->randomString());
        }
	cout<<"-----------------------------"<<endl; 
	sl->RangeQuery("0", node_count);
	cout<<"-----------------------------"<<endl;
	if(sl->get_impl("a") != nullptr)
	  cout<<sl->get("a")<<endl;
	else
	  cout<<"nullptr"<<endl;
   }
	

   else if(type == "JellyFishSkipList"){ 
        sl = new JellyFishSkipList(max_level, node_count);
        for (int i=0;i<node_count; i++){
       cout<<"-----------------------------"<<endl;
	sl->put("kkk",sl->randomString());
        }
	cout<<"-----------------------------"<<endl; 
	sl->RangeQuery("a", node_count);
	cout<<"-----------------------------"<<endl;
	if(sl->get_impl("kkk") != nullptr)
	  cout<<sl->get("kkk")<<endl;
	else
	  cout<<"nullptr"<<endl;
   }
    return 0;
}




 
