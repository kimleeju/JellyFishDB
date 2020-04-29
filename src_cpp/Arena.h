#ifndef ARENA_H
#define ARENA_H

#include<iostream>
#include<atomic>
#include<memory>
using namespace std;
#define ARENA_SIZE 8*1024*1024
#if 1
class Arena{
	public: 
		Arena(){
			arena = new std::atomic<char>[ARENA_SIZE];
			remain_size = ARENA_SIZE;		
		}
		
		std::atomic<char>* AllocateAligned(size_t bytes){
			if(remain_size < bytes){
				arena = new std::atomic<char>[ARENA_SIZE];
				remain_size = ARENA_SIZE;
			}
		
			std::atomic<char>* return_arena;	
			return_arena = arena;
			remain_size-=bytes;
			arena+=bytes;
			//return_arena=(arena[0]).load(std::memory_order_acquire);
			//remain_size-=bytes;
			//(&arena)->store(arena+bytes,std::memory_order_relaxed);	
			return return_arena;

		}

		size_t remain_size;	
		std::atomic<char>* arena;
};
#endif

#if 0
class Arena{
	public: 
		Arena(){
			arena = new char[ARENA_SIZE];
			remain_size = ARENA_SIZE;		
		}
		
		char* AllocateAligned(size_t bytes){
			if(remain_size < bytes){
				arena = new char[ARENA_SIZE];
				remain_size = ARENA_SIZE;
			}
			(&return_arena)->store((&arena)->load(std::memory_order_acquire),std::memory_order_relaxed);
			remain_size-=bytes;
			(&arena)->store(arena+bytes,std::memory_order_relaxed);	
			return return_arena;

		}
		
		std::atomic<char*> return_arena;
		size_t remain_size;	
		std::atomic<char*> arena;
};
#endif


#if 0
class Arena{
	public: 
		Arena(){
			(&arena)->store(new char [ARENA_SIZE],memory_order_relaxed);
			remain_size = ARENA_SIZE;		
		}
		
		char* AllocateAligned(size_t bytes){
			if(remain_size < bytes){
				(&arena)->store(new char [ARENA_SIZE],memory_order_relaxed);
				remain_size = ARENA_SIZE;
			}
			
			(&return_arena)->store(arena,memory_order_relaxed);
			remain_size-=bytes;
			(&arena)->store(arena+bytes,std::memory_order_relaxed);	
			return return_arena;

		}
		
		atomic<char*> return_arena;
		size_t remain_size;	
		atomic<char*> arena;
};
#endif

#if 0
class Arena{
	public: 
		Arena(){
			arena = new std::atomic<char*>[ARENA_SIZE];
			remain_size = ARENA_SIZE;		
		}
		
		char* AllocateAligned(size_t bytes){
			if(remain_size < bytes){
				arena = new std::atomic<char*>[ARENA_SIZE];
				remain_size = ARENA_SIZE;
			}
			char* return_arena = arena->load(std::memory_order_acquire);
			remain_size-=bytes;
			(arena).store(arena+bytes,std::memory_order_relaxed);	
			return return_arena;

		}
		
		size_t remain_size;	
		std::atomic<char*>* arena;
};
#endif

#endif
