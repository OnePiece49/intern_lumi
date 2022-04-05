.PHONY :  HTTPserver.c clean

httpServer:
	gcc -o httpServer HTTPserver.c http-parser/http_parser.c -luv -pthread -I.

clean:
	rm -rf HTTPserver 
