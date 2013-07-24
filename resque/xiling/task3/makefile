all: map_contains
    
map_contains: map_contains_hdfs.cpp 
	g++ -L /usr/local/lib/ -lgeos -I ${HADOOP_HOME}/src/c++/libhdfs/ -I /System/Library/Frameworks/JavaVM.framework/Headers/ -L /usr/local/lib/ -lhdfs map_contains_hdfs.cpp -o map_contains
clean:
	rm -f map_contains
