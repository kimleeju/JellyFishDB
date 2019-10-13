#include "BenchManager.h"

int BenchManager::_read_trc_file(string fname)
{
	DEBUG(__func__);

    string line, tok; 
    ifstream ifs; 
	int ops = 0;

	// open file 	
	ifs.open(fname.c_str());
	if(!ifs.is_open()){
		cout << "Failed to open file: " << fname << endl;
		return -1;
	}

	// read file 
    while(!ifs.eof()){
		ops++;
        getline(ifs, line);

    	stringstream ss(line);
		vector<string> tokens;

        while (ss >> tok) {
            tokens.push_back(tok);
        }

		if(tokens.size() != 2)
			continue;

		// set thread requests 
    	int tid = ops % th;
    	//int tid = stoi(tokens[0]);
		Request rq(tokens[0], tokens[1]);
		
		workers[tid].add_rq(rq); 

    }
    return ops;
}

void* Worker::do_query_with_trace()
{
	DEBUG(__func__);

	if(!sl){
		cout << "Skip List is not created" << endl;
		return NULL;
	}

	Iterator *iter = new Iterator(sl, th);
	sl->SetThreadNum(th); // for JDK SkipList
			
	ops = 0;

	for(vector<Request>::iterator p = rqs.begin(); p != rqs.end(); ++p){
		ops++;
		
		Request rq = *p;
		string val("abcdefghijklmnopqrstuvwzABCDEFGHIJKLMNOPQRSTUVWZ");

		DEBUG(rq.op << " " << rq.key );

		if (rq.op == "put" || rq.op == "PUT" || 
			rq.op == "update" || rq.op == "UPDATE") {
			sl->Put(rq.key, val, *iter);

		} else if (rq.op == "get" || rq.op == "GET" ) {
			val = sl->Get(rq.key, *iter); 

		} else if (rq.op == "range_query" || rq.op == "SEEK") {
			sl->RangeQuery(rq.key, 10, *iter); // op, cnt, iter 
		}

	}
	DEBUG( "completed ops = " << ops );
	
	return NULL;
}



int BenchManager::run_trc(string fname)
{
	DEBUG(__func__);

	for(int i = 0; i < th; i++)
		workers[i].reset();

	// read trc file 
	tot_ops = _read_trc_file(fname);
	if(tot_ops < 0)
		return -1;

	// timer start 
	timer.start();
	// create and run threads 
	for(int i = 0; i < th; i++){
		workers[i].create(i);
	}

	// join thread
	for(int i = 0; i < th; i++){
		workers[i].join();
	}
	//usleep(1000000);

	// time end 
	timer.end();

	double lat = timer.lat();
	// perf_report 
	cout << type 
		<< " workload = " << fname 
		<< " th = " << th  
		<< " tot_ops = " << tot_ops 
		<< " exec_time(s) = " << lat
		<< " IOPS = " << tot_ops / lat 
		<< endl;

	return 0;
}


