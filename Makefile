CC=g++
CXXFLAGS=-std=gnu++11 -std=c++11 -g -Wall
OBJS=Main.o BlockedSkipList.o ConcurrentSkipList.o JellyFishSkipList.o SimpleSkipList.o
OBJS=Main.o SimpleSkipList.o
TARGET=Run

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)   

clean:
	rm $(TARGET) $(OBJS)
