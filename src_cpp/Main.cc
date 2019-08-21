#define DEBUG
#include "SkipList.h"
#include "BlockedSkipList.h"
#include "ConcurrentSkipList.h"
#include "JellyFishSkipList.h"
#include "SimpleSkipList.h"
#include "Workload.h"
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
//#include <thread>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define MAXVALUE 100
struct MultipleArg {
	vector<vector<Workloads> > &v;
	int index;
	SkipList *sl;
};

struct timeval t_start;
struct timeval t_end;

void print_result(){
	printf("%6.2f\n", (t_end.tv_sec - t_start.tv_sec + (t_end.tv_usec - t_start.tv_usec) / 1000000.0));
	return ;
}
void *do_query_with_trace(void *multiple_arg) {
	MultipleArg *my_multiple_arg = (MultipleArg *)multiple_arg;
	int value;
	string operation;
	multimap<string, string> m;
	string getvalue;
	for (unsigned int j = 0; j < my_multiple_arg->v[my_multiple_arg->index].size(); j++) {
		operation = my_multiple_arg->v[my_multiple_arg->index][j].getOperation();
		if (operation == "put" || operation == "update") {
			value = rand() % MAXVALUE;
			if (operation == "put") {
				my_multiple_arg->sl->put(my_multiple_arg->v[my_multiple_arg->index][j].getKey(), to_string(value));
			}
			else if (operation == "update") {
				// 해당 key가 없으면 update안함
				if (my_multiple_arg->sl->get(my_multiple_arg->v[my_multiple_arg->index][j].getKey()) != "not found") {
					printf("[update]  ");
					my_multiple_arg->sl->put(my_multiple_arg->v[my_multiple_arg->index][j].getKey(), to_string(value));
				}
				else { cout << "[update fail]" << endl; }
			}
		}
		else if (operation == "get") {
			getvalue = my_multiple_arg->sl->get(my_multiple_arg->v[my_multiple_arg->index][j].getKey());
			if (getvalue == "not found") {
				printf("[not found key]\n");
			}
			else {
				printf("[get( %s  %s ) success]\n", my_multiple_arg->v[my_multiple_arg->index][j].getKey().c_str(), getvalue.c_str());
			}
		}
		else if (operation == "scan") {
			m = my_multiple_arg->sl->range_query(my_multiple_arg->v[my_multiple_arg->index][j].getKey(), my_multiple_arg->v[my_multiple_arg->index][j].getCount());
			multimap<string, string>::iterator iter;
			for (iter = m.begin(); iter != m.end(); iter++) {
				cout << iter->first << "  " << iter->second << endl;
			}
		}
	}
}
/*
   void *do_query_with_trace(void *multiple_arg) {
   MultipleArg *my_multiple_arg = (MultipleArg *)multiple_arg;
   int value;
   string operation;
   multimap<string, string> m;
   string getvalue;
   for (unsigned int j = 0; j < my_multiple_arg->v.size(); j++) {
   operation = my_multiple_arg->v[j].getOperation();
   if (operation == "put" || operation == "update") {
   value = rand() % MAXVALUE;
   if (operation == "put") {
   my_multiple_arg->sl->put(my_multiple_arg->v[j].getKey(), to_string(value));
   }
   else if (operation == "update") {
// 해당 key가 없으면 update안함
if (my_multiple_arg->sl->get(my_multiple_arg->v[j].getKey()) != "not found") {
printf("[update]  ");
my_multiple_arg->sl->put(my_multiple_arg->v[j].getKey(), to_string(value));
}
else { cout << "[update fail]" << endl; }
}
}
else if (operation == "get") {
getvalue = my_multiple_arg->sl->get(my_multiple_arg->v[j].getKey());
if (getvalue == "not found") {
printf("[not found key]\n");
}
else {
printf("[get( %s  %s ) success]\n", my_multiple_arg->v[j].getKey().c_str(), getvalue.c_str());
}
}
else if (operation == "scan") {
m = my_multiple_arg->sl->range_query(my_multiple_arg->v[j].getKey(), my_multiple_arg->v[j].getCount());
multimap<string, string>::iterator iter;
for (iter = m.begin(); iter != m.end(); iter++) {
cout << iter->first << "  " << iter->second << endl;
}
}
}
}
 */
int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	SkipList* sl;

	if (argc < 4) {
		cout << "Usage: ./Run ConcurrentSkipList thread_num trace_path" << endl;
		cout << "Options: BlockedSkipList ConcurrentSkipList JellyFishSkipList" << endl;
		return -1;
	}

	string type(argv[1]);
	cout << "type: " << type << endl;

	if (type == "BlockedSkipList") {
		//sl = new BlockedSkipList();

	}
	else if (type == "ConcurrentSkipList") {
		//sl = new ConcurrentSkipList();

	}
	else if (type == "JellyFishSkipList") {
		//sl = new JellyFishSkipList();
	}
	else if (type == "SimpleSkipList") {
		sl = new SimpleSkipList();
	}
	int threadnum = atoi(argv[2]);

	ifstream file;
	string str;
	char *filepath = argv[3];
	// save file
	vector<vector<Workloads> > workload(threadnum);

	//file.open("C:\\lsh\\t.txt", ios::in);
	file.open(filepath, ios::in);
	if (!file.is_open()) {
		cout << "Error opening file" << endl;
		exit(1);
	}
	int sindex, eindex;
	string temp;
	int count = 0;
	while (!file.eof()) {
		Workloads th;
		sindex = 0;
		eindex = 0;
		count = 0;

		getline(file, str);
		if(!str.empty()){
			for (unsigned int i = 0; i < str.size(); i++) {
				if (str[i] == ' ' || i + 1 == str.size()) {
					if (i + 1 == str.size()) { eindex++; }
					temp.clear();
					temp.assign(str, sindex, eindex);
					count++;
					if (count == 1) { th.setTid(stoi(temp)); sindex = sindex + eindex + 1; eindex = 0; }
					else if (count == 2) { th.setOperation(temp); sindex = sindex + eindex + 1; eindex = 0; }
					else if (count == 3) { th.setKey(temp); sindex = sindex + eindex + 1; eindex = 0; }
					else if (count == 4) {
						th.setCount(stoi(temp)); sindex = sindex + eindex + 1; eindex = 0;
					}
					else { count = 0; break; }
				}
				else { eindex++; }
			}
			if (th.getOperation() != "scan") { th.setCount(0);  eindex = 0; }

			for (unsigned int i = 0; i < workload.size(); i++) {
				// 비어있지 않고 tid가 같은 곳이 있으면 거기에 push back
				if (!(workload[i].empty())) {
					if (workload[i][0].getTid() == th.getTid()) {
						workload[i].push_back(th);
						break;
					}
				}
				else { workload[i].push_back(th); break; }  // 앞에서 부터 확인하는데 비어있으면 추가
			}
		}
	}

	for (unsigned int i = 0; i < workload.size(); i++) {
		cout << "workload[" << i << "] 의 size  "<<workload[i].size() << endl;
		for (unsigned int j = 0; j < workload[i].size(); j++) {
			cout << workload[i][j].getTid() << " " << workload[i][j].getOperation() << " " << workload[i][j].getKey() << " " << workload[i][j].getCount() << endl;
		}
	}

	file.close();

//	MultipleArg multiple_arg;
//	multiple_arg = (MultipleArg *)malloc(sizeof(MultipleArg));
	struct MultipleArg *multiple_arg;
	multiple_arg = (struct MultipleArg*)malloc(sizeof(struct MultipleArg));
	pthread_t work[threadnum];
	multiple_arg->sl = sl;
	multiple_arg->v = workload;

	//time start
	if(gettimeofday(&t_start, NULL)==-1){
		printf("failed to read time\n");
		return -1;
	}
	for (int i = 0; i < threadnum; i++) {
		multiple_arg->index = i;
		pthread_create(&work[i], NULL, do_query_with_trace, (void *)multiple_arg);
	}

	for (int i = 0; i < threadnum; i++) {
		pthread_join(work[i], NULL);
	}
	//time end
	if(gettimeofday(&t_end, NULL)==-1){
		printf("failed to read time\n");
		return -1;
	}
	/*
	   vector<thread> work;
	   for (int i = 0; i < 4; i++) {
	   work.push_back(thread(do_query_with_trace,workload[i], sl));
	   }

	   for (int i = 0; i < 4; i++) {
	   work[i].join();
	   }
	 */
	//	sl->print();
	print_result();

	return 0;
}

