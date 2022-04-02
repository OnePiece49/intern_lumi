.PHONY : httpserver newHttpserver  clean

httpserver:
	 gcc -o httpserver httpserver.c ./http-parser/http_parser.c -luv -pthread

newHttpserver:
	gcc -o newHttpserver newHttpserver.c ./http-parser/http_parser.c -luv -pthread

clean:
	rm -rf httpserver
