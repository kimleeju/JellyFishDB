javac MyConcurrentSkipListMap.java


g++ -g -I/usr/lib/jvm/java-8-openjdk-amd64/include/ \
	-I/usr/lib/jvm/java-8-openjdk-amd64/include/linux/ \
	-L/usr/bin/java \
	-L/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/ JNI_ConcurrentSkipList.cc -ljvm
	#-L/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/ JNI_ConcurrentSkipList.cc -ljvm

./a.out

