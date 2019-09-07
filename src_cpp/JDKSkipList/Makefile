CC=g++
CXXFLAGS=-std=c++11 -g -Wall \
	-g -I/usr/lib/jvm/java-8-openjdk-amd64/include/ \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux/ \
	-L/usr/bin/java \
	-L/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/

#LDFLAGS= -pthread
SRCS +=\
	JNI_ConcurrentSkipList.cc\
	
OBJS:= $(SRCS:.cc=.o)


all: $(SRCS)
	g++ $(CXXFLAGS) -o Run $(SRCS) -ljvm

#all: $(OBJS)
#	$(CC) $(LDFLAGS) -o $@ -c $^

clean:
	rm -f *.o Run
