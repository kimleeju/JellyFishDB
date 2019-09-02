#include "Bench.h"
#include "Util.h"
#include <stdio.h>
#include <cstring>
#define MAXVALUE 100

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
void *Bench::do_query_with_trace() {
	for (unsigned int j = 0; j < t_arg->wl_th.size(); j++) {
	//for (unsigned int j = 0; j < 10; j++) {
		set_op(t_arg->wl_th[j].getOp());
		if (get_op() == "put" || get_op() == "update") {
			set_val(rand() % MAXVALUE);
			if (get_op() == "put") {
				t_arg->sl->put(t_arg->wl_th[j].getKey(), to_string(get_val()));
			}
			else if (get_op() == "update") {
				// 해당 key가 없으면 update안함
				//if (t_arg->sl->get(t_arg->wl_th[j].getKey()) != "not found") {
				//	printf("[update]  ");
					t_arg->sl->put(t_arg->wl_th[j].getKey(), to_string(get_val()));
				//}
				//else { cout << "[update fail]" << endl; }
			}
		}
		else if (get_op() == "get") {
			set_rv(t_arg->sl->get(t_arg->wl_th[j].getKey()));
			if (get_rv() == "not found") {
				//printf("[not found key]\n");
			}
			else {
				//printf("[get( %s  %s ) success]\n", t_arg->wl_th[j].getKey().c_str(), get_rv().c_str());
			}
		}
		else if (get_op() == "scan") {
			t_arg->sl->RangeQuery(t_arg->wl_th[j].getKey(), t_arg->wl_th[j].getCnt());
			//multimap<string, string>::iterator iter;
			/*for (iter = mmap.begin(); iter != mmap.end(); iter++) {
				cout << iter->first << "  " << iter->second << endl;
			}*/
		}
	}
	
	return NULL;
}
