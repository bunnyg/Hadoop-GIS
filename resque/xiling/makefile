all: resque 
    
resque: resque.cpp 
	g++ -L/usr/local/lib/ -lgeos resque.cpp -o resque 
clean:
	rm -f resque
