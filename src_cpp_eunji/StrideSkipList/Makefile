CC=g++
CXXFLAGS=-std=gnu++11 -std=c++11 -g -Wall
LDFLAGS= -pthread

SRCS +=\
	StrideSkipList.cc\
	
OBJS:= $(SRCS:.cc=.o)

all: $(OBJS)
	$(CC) $(LDFLAGS) -c $< -o $@

clean:
	rm -f *.o
