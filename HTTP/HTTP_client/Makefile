.PHONY : build build_test clean test

build: 
	g++ -Wall  -g -o curl client.cpp util.c  -lcurl 

build_test:
	g++ -Wall  -g -o build_test  test_curl.cpp  -lcurl

clean:
	rm -rf curl 

test: 
	./curl https://freegeoip.app/json/ 
