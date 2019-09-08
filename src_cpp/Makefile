CC=g++
CXXFLAGS=-std=c++11 -g -Wall
LDFLAGS=-pthread

SRCS +=\
	BlockedSkipList/BlockedSkipList.cc\
	ConcurrentSkipList/ConcurrentSkipList.cc\
	JDKSkipList/JNISkipList.cc\
	JellyFishSkipList/JellyFishSkipList.cc\
	Bench/BenchManager.cc

OBJS = $(SRCS:.cc=.o)

$(info $(OBJS))

TARG=Run

all: $(TARG)

$(TARG): Main.cc ./bin/libskiplist.a
	$(CC) $(LDFLAGS) $(CXXFLAGS) -o $@ $^

./bin/libskiplist.a: $(SRCS)
	mkdir -p bin
	cd ./BlockedSkipList && $(MAKE) clean && $(MAKE) && cd ..
	cd ./ConcurrentSkipList && $(MAKE) clean && $(MAKE) && cd ..
	cd ./JDKSkipList && $(MAKE) clean && $(MAKE) && cd ..
	cd ./JellyFishSkipList && $(MAKE) clean && $(MAKE) && cd ..
	cd ./Bench && $(MAKE) clean && $(MAKE) && cd ..
	$(AR) r $(@) $(OBJS)

clean:
	rm -f $(TARG) $(OBJS) ./bin/*.o ./bin/libskiplist.a
