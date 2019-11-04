#ifndef WORKLOAD_H
#define WORKLOAD_H
#include <vector>
#include <string>
using namespace std;

class Workloads {
	public:
		void setTid(int t);
		void setKey(string k);
		void setOp(string o);
		void setCnt(int c);
		int getTid();
		string getKey();
		string getOp();
		int getCnt();

		void save_workloads(vector<vector<Workloads> > &v, char *p);
	private:
		int tid;
		string key;
		string op;
		int cnt;
};

#endif
