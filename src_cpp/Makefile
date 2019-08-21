CC=g++
LDLIBS= -lpthread
CFLAGS=-g -Wall -std=c++11
OBJS=Main.o BlockedSkipList.o ConcurrentSkipList.o JellyFishSkipList.o SimpleSkipList.o
OBJS=Main.o SimpleSkipList.o
TARGET=Run

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDLIBS)

clean:
	rm $(TARGET) $(OBJS)
