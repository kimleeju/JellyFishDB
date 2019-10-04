#include "BenchManager.h"

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
						//cout<<"before count = "<<th.getCnt()<<endl;
						th.setCnt(stoi(temp)); sindex = sindex + eindex + 1; eindex = 0;
//						cout<<"after count = "<<th.getCnt()<<endl;
					}
					else { cnt = 0; break; }
				}
				else { eindex++; }
			}
//			if (th.getOp() != "range_query" || th.getOp() != "SEEK") { th.setCnt(0);  eindex = 0; }

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

void Bench::set_val(int i) { val = i; }
void Bench::set_op(string o) { op = o; }
void Bench::set_rv(string r) { get_v = r; }
void Bench::set_req(Request *r){ t_arg = r; }

int Bench::get_val() { return val; }
string Bench::get_op() { return op; }
string Bench::get_rv(){ return get_v; }
Request *Bench::get_req(){ return t_arg;}
void Bench::printf_req(){
	for(unsigned int i=0;i<t_arg->wl_th.size();i++){
		printf("--%d  %s  %s  %d\n", t_arg->wl_th[i].getTid(), t_arg->wl_th[i].getOp().c_str(), t_arg->wl_th[i].getKey().c_str(), t_arg->wl_th[i].getCnt());
	}
}
void *Bench::do_query_with_trace(int seq) {
//	Iterator iterator(t_arg->sl);
//	printf_req();
	Iterator* iterator = new Iterator(t_arg->sl,seq);
	t_arg->sl->GetEnv(seq);
	for (unsigned int j = 0; j < t_arg->wl_th.size(); j++) {
		set_op(t_arg->wl_th[j].getOp());
//	cout<<"1111111"<endl;
		if (get_op() == "put" || get_op() == "update"||get_op() == "PUT") {
			set_val(rand() % MAXVALUE);
			if (get_op() == "put" || get_op()=="PUT") {
				t_arg->sl->Put(t_arg->wl_th[j].getKey(), to_string(get_val()), *iterator);
			}
		//	else if (get_op() == "update") {
				// 해당 key가 없으면 update안함
				//if (t_arg->sl->get(t_arg->wl_th[j].getKey()) != "not found") {
				//	printf("[update]  ");
		//			t_arg->sl->Put(t_arg->wl_th[j].getKey(), to_string(get_val()), *iterator);
				//}
				//else { cout << "[update fail]" << endl; }
		//	}
		}
		else if (get_op() == "get"||get_op()=="GET") {
			set_rv(t_arg->sl->Get(t_arg->wl_th[j].getKey(), *iterator));
			if (get_rv() == "not found") {
				//printf("[not found key]\n");
			}
			else {
				//printf("[get( %s  %s ) success]\n", t_arg->wl_th[j].getKey().c_str(), get_rv().c_str());
			}
		}
		else if (get_op() == "range_query"||get_op() == "SEEK") {
//			cout<<"count = "<<t_arg->wl_th[j].getCnt()<<endl;
			t_arg->sl->RangeQuery(t_arg->wl_th[j].getKey(), t_arg->wl_th[j].getCnt(),*iterator);
			//mmap = t_arg->sl->RangeQuery(t_arg->wl_th[j].getKey(), t_arg->wl_th[j].getCnt());
			multimap<string, string>::iterator iter;
			/*for (iter = mmap.begin(); iter != mmap.end(); iter++) {
				cout << iter->first << "  " << iter->second << endl;
			}*/
		}
	}
	
	return NULL;
}


BenchManager::BenchManager(int t, char *lp, char *rp, SkipList *t_s) : th_num(t), l_path(lp), r_path(rp), sl(t_s) {
	l_vec.resize(t);
	r_vec.resize(t);
	gnrtor = new Generator[t];
	//req = (Request *)malloc(sizeof(Request) * t);
	req = new Request[t];
	req->sl = sl;
	for(int i = 0; i < t ; i++){
		req[i].sl = sl;
		gnrtor[i].seq= i;
	}
}

void BenchManager::prepare(){
	//	r->sl = s;
}/*
    void BenchManager::print_vector(){
	r->v.assign(v[0].begin(), v[0].end());
	cout<<"workload[0] : 의 size"<<v[0].size()<<endl;
	for (unsigned int i = 0; i < v[0].size(); i++) {
	//	cout << "workload[" << i << "] 의 size  "<<v[i].size() << endl;
		//for (unsigned int j = 0; j < v[i].size(); j++) {
			cout << v[0][i].getTid() << " " << v[0][i].getOperation() << " " << v[0][i].getKey() << " " << v[0][i].getCount() << endl;
	//	}
	}
}*/
void BenchManager::load_trc(){
	wl.save_workloads(l_vec,l_path);

	// set benmark for each thread
	for(int i=0;i<th_num;i++){
		Bench *bnch = new Bench;
		req[i].wl_th = l_vec[i];
 		bnch->set_req(&req[i]);  // save request each Bench class
		gnrtor[i].set_bench(bnch);
	}
	for(int i=0;i<th_num; i++){
		gnrtor[i].create();
	}
	// join thread
	for(int i=0; i<th_num; i++){
		gnrtor[i].join();
	}

}

unsigned long BenchManager::run_trc(){
	wl.save_workloads(r_vec,r_path);
	class Time time;

	// time start
	if(gettimeofday(&time.get_start(), NULL)==-1){
		cout<<"failed to read time"<<endl;
		return 1;
	}
	// set benmark for each thread
	unsigned long op_cnt=0;
	for(int i=0;i<th_num;i++){
		Bench *bnch = new Bench;
		req[i].wl_th = r_vec[i];
		op_cnt += r_vec[i].size();
 		bnch->set_req(&req[i]);  // save request each Bench class
		gnrtor[i].set_bench(bnch);
	}
	for(int i=0;i<th_num; i++){
		gnrtor[i].create();	
	}
	// join thread
	for(int i=0; i<th_num; i++){
		gnrtor[i].join();
	}

	// time end
	if(gettimeofday(&time.get_end(), NULL)==-1){
		cout<<"failed to read time"<<endl;
		return 1;
	}

	//----------------------------------------//
	//----- print perf -----------------------//
	//----------------------------------------//
	//time.print_result();
	time.get_dur();
	return op_cnt/time.get_dur();
}
void BenchManager::get_stat(){
}
