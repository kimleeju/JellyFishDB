CC=g++
CXXFLAGS=-std=c++11 -g -Wall \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include/ \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux/ \
	-L/usr/bin/java \
	-L/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/

LDFLAGS= -pthread
SRCS +=\
	JDKSkipList.cc
	
OBJS:= $(SRCS:.cc=.o)


#all: $(SRCS)
#	$(cc) $(CXXFLAGS) -o Run $(SRCS) -ljvm

#all: $(OBJS)
#	$(cc) $(LDFLAGS) -c $< -o $@

all: $(OBJS)
#	javac ThreadLocalRandom.java
#	javac ConcurrentSkipListMap.java
	javac MyConcurrentSkipListMap.java
#	cp MyConcurrentSkipListMap.class ..
	cp *.class ..
	$(CC) $(LDFLAGS) $(CXXFLAGS) -o $@ -c $^ -ljvm

clean:
	rm -f *.o Run
