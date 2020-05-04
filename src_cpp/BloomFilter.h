#ifndef BLOOMFILTER
#define BLOOMFILTER
#include<iostream>
#include<string>
#include<stdlib.h>
//#define BLOOM_SIZE 1000000
#define BLOOM_ENTRIES 1024 * 1024 
//#define BLOOM_ENTRIES 10000000000
#define BLOOM_ENTRIES_BITS 20
#define BITS_FOR_ENTRY 8
using namespace std;

	class BloomFilter{

		public:	
			BloomFilter(){
				num = new char[BLOOM_ENTRIES/BITS_FOR_ENTRY + 1];
			}
			
			void Insert(string& key){
				long long idx, offset;
				long long _key = stoll(key);	
				//_key=_key % BLOOM_ENTRIES; 
				_key = _key & ((1 << 20) - 1);
				//idx = _key / BITS_FOR_ENTRY;
				idx = _key >> 3;
				//offset = _key % BITS_FOR_ENTRY;
				offset = _key & 7;
				num[idx] != (1 << offset);
				//setBit(idx,offset);
			}
	
			bool getBit(const string& key){
				long long idx, offset;
				long long _key = stoll(key);
				//_key=_key % BLOOM_ENTRIES; 
				_key = _key & ((1 << 20) - 1);
				//idx = _key / BITS_FOR_ENTRY;
				idx = _key >> 3;
				//offset = _key % BITS_FOR_ENTRY;
				offset = _key & 7;
				return ((num[idx] & (1 <<offset)));
			}

			bool setBit(long long & idx, long long & offset){
				return num[idx] |= (1 << (offset));
			}
		private:
			char* num;
			
	};



#endif
