#pragma once
#ifndef SimpleSKIPLIST_H
#define SimpleSKIPLIST_H

#include "SkipList.h"

class SimpleSkipList : public SkipList {

	private:
		multimap<string, string> kv_map; // hash map 

	public:
		int put(string key, string value);
		string get(string key);
		int remove(string key);
		multimap<string, string> range_query(string starting_key, int n);
		void print();
	public:
		SimpleSkipList() {}
		~SimpleSkipList() {}
};

#endif
