CC=g++

CXXFLAGS=-std=c++11 -g -Wall -O0

LDFLAGS=-pthread

SRCS +=\
	BlockedSkipList/BlockedSkipList.cc\
	CVSkipList/CVSkipList.cc\
	ConcurrentSkipList/ConcurrentSkipList.cc\
	JellyFishSkipList/JellyFishSkipList.cc\
	Bench/BenchManager.cc


OBJS = $(SRCS:.cc=.o)

$(info $(OBJS))

TARG=Run

all: $(TARG)

$(TARG): Main.cc ./bin/libskiplist.a
	$(CC) $(LDFLAGS) $(CXXFLAGS) -o $@ $^

#$(CC) $(LDFLAGS) $(CXXFLAGS) -o $@ $^ -ljvm
./bin/libskiplist.a: $(SRCS)
	mkdir -p bin
	cd ./BlockedSkipList && $(MAKE) clean && $(MAKE) && cd ..
	cd ./CVSkipList && $(MAKE) clean && $(MAKE) && cd ..
	cd ./ConcurrentSkipList && $(MAKE) clean && $(MAKE) && cd ..
	cd ./JellyFishSkipList && $(MAKE) clean && $(MAKE) && cd ..
	cd ./Bench && $(MAKE) clean && $(MAKE) && cd ..
	$(AR) r $(@) $(OBJS)

clean:
	rm -f $(TARG) $(OBJS) ./bin/*.o ./bin/libskiplist.a
#	cd ./JDKSkipList && $(MAKE) clean && $(MAKE) && cd ..
#	JDKSkipList/JDKSkipList.cc\
# CXXFLAGS=-std=c++11 -g -Wall \
	-g -I/usr/lib/jvm/java-8-openjdk-amd64/include/ \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux/ \
	-L/usr/bin/java \
	-L/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/


