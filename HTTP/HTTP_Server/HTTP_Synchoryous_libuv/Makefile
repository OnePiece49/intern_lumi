.PHONY :  httpServer clean

httpServer:
	g++ -o httpServer httpServer.cpp ./lib/http.cpp -luv -pthread -I./lib

clean:
	rm -rf httpServer newHttpserver
