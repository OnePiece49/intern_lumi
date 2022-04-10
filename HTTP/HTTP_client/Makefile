.PHONY : build build_test clean test

build_test:
	g++ -Wall  -g -o build_test  test_curl.cpp  -lcurl

clean:
	rm -rf curl build_test

test: 
	./curl https://freegeoip.app/json/ 
