#ifndef TIME_H
#define TIME_H
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
class Time{
	public:
		timeval &get_start(){ return t_start; }
		timeval &get_end(){ return t_end; }
		double get_dur(){
			return t_end.tv_sec-t_start.tv_sec + (t_end.tv_usec - t_start.tv_usec) / 1000000.0;
		}

		void print_result(){
			printf("%6.2f\n", (t_end.tv_sec-t_start.tv_sec + (t_end.tv_usec - t_start.tv_usec) / 1000000.0));
		}

	private:
		struct timeval t_start;
		struct timeval t_end;
};

#endif
