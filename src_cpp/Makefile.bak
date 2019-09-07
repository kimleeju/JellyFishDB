CC=g++
CXXFLAGS=-std=gnu++11 -std=c++11 -g -Wall
LDFLAGS= -pthread
OBJS=Main.o SkipList.o ConcurrentSkipList.o JellyFishSkipList.o SimpleSkipList.o BlockedSkipList.o WorkLoad.o Bench.o BenchManager.o Thread.o
TARGET=Run

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)   

clean:
	rm $(TARGET) $(OBJS)
