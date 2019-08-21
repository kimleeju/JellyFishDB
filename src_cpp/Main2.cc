#define DEBUG
#include <iostream>
using namespace std;
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
#include <thread>
#include <cstdlib>
#define MAXVALUE 100

struct MultipleArg{
	vector<Workloads> v;
	SkipList sl;
};

void *do_query_with_trace(void *multiple_arg) {
	int value;
	string operation;
	multimap<string, string> m;
	string getvalue;
	for (unsigned int j = 0; j < v.size(); j++) {
		operation = v[j].getOperation();
		if (operation == "put" || operation == "update") {
			value = rand() % MAXVALUE;
			if (operation == "put") {
				sl->put(v[j].getKey(), to_string(value));
			}
			else if (operation == "update") {
				// 해당 key가 없으면 update안함
				if (sl->get(v[j].getKey()) != "not found") {
					printf("[update]  ");
					sl->put(v[j].getKey(), to_string(value));
				}
				else { cout << "[update fail]" << endl; }
			}
		}
		else if (operation == "get") {
			getvalue = sl->get(v[j].getKey());
			if (getvalue == "not found") {
				printf("[not found key]\n");
			}
			else {
				printf("[get( %s  %s ) success]\n", v[j].getKey().c_str(), getvalue.c_str());
			}
		}
		else if (operation == "scan") {
			m = sl->range_query(v[j].getKey(), v[j].getCount());
			multimap<string, string>::iterator iter;
			for (iter = m.begin(); iter != m.end(); iter++) {
				cout << iter->first << "  " << iter->second << endl;
			}
		}
	}
}

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

	ifstream file;
	string str;
	char *filepath = argv[3];
	// save file
	vector<vector<Workloads> > workload(4);

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

	for (unsigned int i = 0; i < workload.size(); i++) {
		cout << "workload[" << i << "]" << endl;
		for (unsigned int j = 0; j < workload[i].size(); j++) {
			cout << workload[i][j].getTid() << " " << workload[i][j].getOperation() << " " << workload[i][j].getKey() << " " << workload[i][j].getCount() << endl;
		}
	}

	file.close();


	std::vector<std::thread> work;
	for (int i = 0; i < 4; i++) {
		work.push_back(thread(do_query_with_trace,workload[i], sl));
	}

	for (int i = 0; i < 4; i++) {
		work[i].join();
	}
/*
	int threadnum=0;
	threadnum = atoi(argv[2]);
	vector<pthread_t> work(threadnum);

	for(int i=0;i<4;i++){
		pthread_create(&work[i], NULL, do_query_with_trace,(void *)workload[i], (void *)sl); 
	}
	for(int i=0;i<4;i++){
		pthread_join(work[i], NULL);
	}
	*/
	sl->print();

	return 0;
}

