.PHONY : main httpserver testCallback clean

httpserver:
	g++ -o httpserver httpserver.cpp -lmicrohttpd

main:	
	gcc -o main main.c -lmicrohttpd

testCallback:
	g++ -o testCallback testCallback.cpp -lmicrohttpd

clean:
	rm -rf httpserver
	rm -rf main

