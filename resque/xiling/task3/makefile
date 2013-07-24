all: map_contains
    
map_contains: map_contains_hdfs.cpp 
	g++ -L /usr/local/lib/ -lgeos -lhdfs map_contains_hdfs.cpp -o map_contains
clean:
	rm -f map_contains
