#ifndef TIME_H
#define TIME_H
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <iostream>

using namespace std;
using namespace chrono;

class Timer {
	public:
		void start(){
			gettimeofday(&t_start, NULL);
		}
		void end(){
			gettimeofday(&t_end, NULL);
		}

		double lat(){
			return t_end.tv_sec-t_start.tv_sec + (t_end.tv_usec - t_start.tv_usec) / 1000000.0;
		}

		void sleep(unsigned s)
		{
			usleep(s * 1000000);
		}

		void print_result(){
			printf("%6.2f\n", (t_end.tv_sec-t_start.tv_sec + (t_end.tv_usec - t_start.tv_usec) / 1000000.0));
		}

	private:
		struct timeval t_start;
		struct timeval t_end;
};

class NanoTimer {
	public:
		void start(){
			t_start = chrono::high_resolution_clock::now();
		}
		void end(){
			t_end = chrono::high_resolution_clock::now();
		}

		long lat(){
			//cout << (t_end-t_start).count() << " s" << endl;
			return (t_end - t_start).count();
		}

		void sleep(unsigned s)
		{
			usleep(s * 1000000);
		}

	private:
		chrono::high_resolution_clock::time_point t_start; 
		chrono::high_resolution_clock::time_point t_end;
};



#endif
