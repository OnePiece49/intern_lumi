.PHONY : httpserver newHttpserver  clean


newHttpserver:
	gcc -o newHttpserver newHttpserver.c ./http-parser/http_parser.c -luv -pthread -I.

httpserver:
	gcc -o httpserver HTTPserver.c ./http-parser/http_parser.c -luv -pthread -I.

clean:
	rm -rf httpserver newHttpserver
