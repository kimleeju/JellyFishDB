#include <string>
using namespace std;

class Workloads {
	public:
		void setTid(int t) { tid = t; }
		void setKey(string k) { key = k; }
		void setOperation(string o) { operation = o; }
		void setCount(int c) { count = c; }
		int getTid() { return tid; }
		string getKey() { return key; }
		string getOperation() { return operation; }
		int getCount() { return count; }
	private:
		int tid;
		string key;
		string operation;
		int count;
};
