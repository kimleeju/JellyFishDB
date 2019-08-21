#pragma once
#ifndef SKIPLIST_H
#define SKIPLIST_H

#include<string>
#include<iostream>
#include<map>
#include<unordered_map>
#include<set>
#include<mutex>
#include <list>
// #include<pthread.h>

#include "Util.h"
using namespace std;

#define MAX_LEVEL 12 

class SkipList {
	protected:
		int height;
		TimeStamp global_ts;
		TimeStamp committed_ts;

	public:
		/**
		 * put, get, delete
		 * */
		virtual int put(string key, string value) = 0;
		virtual string get(string key) = 0;
		virtual int remove(string key) = 0;
		virtual multimap<string, string> range_query (string starting_key, int n) = 0;
		virtual void print() = 0;
	public:
		SkipList() :
			global_ts(0),
			committed_ts(0)
	{};
		~SkipList() {}
};

#endif

