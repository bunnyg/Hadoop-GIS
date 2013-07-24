all: map_contains
    
map_contains: map_contains.cpp 
	g++ -L /usr/local/lib/ -lgeos -lhdfs map_contains.cpp -o map_contains
clean:
	rm -f map_contains
