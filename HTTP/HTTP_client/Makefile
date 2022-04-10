.PHONY : build build_test clean test

build: 
	g++ -Wall  -g -o curl client.cpp ./lib/http.cpp -lcurl -I./lib

build_test:
	g++ -Wall  -g -o build_test  test_curl.cpp  -lcurl

clean:
	rm -rf curl build_test

test: 
	./curl https://freegeoip.app/json/ 
