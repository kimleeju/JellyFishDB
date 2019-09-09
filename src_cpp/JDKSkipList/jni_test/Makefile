CC=g++
CXXFLAGS=-std=c++11 -g -Wall \
	-g -I/usr/lib/jvm/java-8-openjdk-amd64/include/ \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux/ \
	-L/usr/bin/java \
	-L/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/

#LDFLAGS= -pthread
SRCS +=\
	JDKSkipList.cc

OBJS:= $(SRCS:.cc=.o)


#all: $(SRCS)
#	$(cc) $(CXXFLAGS) -o Run $(SRCS) -ljvm

#all: $(OBJS)
#	$(cc) $(LDFLAGS) -c $< -o $@

all: Main.cc JDKSkipList.cc
	g++ $(LDFLAGS) $(CXXFLAGS) -o JDKSkipList.o -c JDKSkipList.cc -ljvm
	$(AR) r libjdk.a JDKSkipList.o 
	g++ $(LDFLAGS) $(CXXFLAGS) -o Run Main.cc libjdk.a -ljvm
	#$(CC) $(LDFLAGS) $(CXXFLAGS) -o Run JDKSkipList.cc Main.cc -ljvm

	#$(CC) $(LDFLAGS) $(CXXFLAGS) -o $@ -c $^ -ljvm

clean:
	rm -f *.o Run
