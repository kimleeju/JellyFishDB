#include "WorkLoad.h"
#include <fstream>
#include <iostream>
using namespace std;

void Workloads::setTid(int t){ tid = t; }
void Workloads::setKey(string k){ key = k; }
void Workloads::setOp(string o){ op = o; }
void Workloads::setCnt(int c){ cnt = c; }
int Workloads::getTid(){ return tid; }
string Workloads::getKey() { return key; } 
string Workloads::getOp() { return op; }
int Workloads::getCnt() { return cnt; }

void Workloads::save_workloads(vector<vector<Workloads> > &v, char *p){
	ifstream file;
	string str;
	char *fp = p;  // filepath

	file.open(fp, ios::in);
	if (!file.is_open()) {
		cout << "Error opening file" << endl;
		exit(1);
	}
	int sindex, eindex;
	string temp;
	int cnt = 0;
	while (!file.eof()) {
		Workloads th;
		sindex = 0;
		eindex = 0;
		cnt = 0;

		getline(file, str);
		if(!str.empty()){
			for (unsigned int i = 0; i < str.size(); i++) {
				if (str[i] == ' ' || i + 1 == str.size()) {
					if (i + 1 == str.size()) { eindex++; }
					temp.clear();
					temp.assign(str, sindex, eindex);
					cnt++;
					if (cnt == 1) { th.setTid(stoi(temp)); sindex = sindex + eindex + 1; eindex = 0; }
					else if (cnt == 2) { th.setOp(temp); sindex = sindex + eindex + 1; eindex = 0; }
					else if (cnt == 3) { th.setKey(temp); sindex = sindex + eindex + 1; eindex = 0; }
					else if (cnt == 4) {
						th.setCnt(stoi(temp)); sindex = sindex + eindex + 1; eindex = 0;
					}
					else { cnt = 0; break; }
				}
				else { eindex++; }
			}
			if (th.getOp() != "scan") { th.setCnt(0);  eindex = 0; }

			for (unsigned int i = 0; i < v.size(); i++) {
				// 비어있지 않고 tid가 같은 곳이 있으면 거기에 push back
				if (!(v[i].empty())) {
					if (v[i][0].getTid() == th.getTid()) {
						v[i].push_back(th);
						break;
					}
				}
				else { v[i].push_back(th); break; }  // 앞에서 부터 확인하는데 비어있으면 추가
			}
		}
	}
/*
	for (unsigned int i = 0; i < v.size(); i++) {
		cout << "workload[" << i << "] 의 size  "<<v[i].size() << endl;
		for (unsigned int j = 0; j < v[i].size(); j++) {
			cout <<"[workload]" << v[i][j].getTid() << " " << v[i][j].getOp() << " " << v[i][j].getKey() << " " << v[i][j].getCnt() << endl;
		}
	}
*/
	file.close();
}
