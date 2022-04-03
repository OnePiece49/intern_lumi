.PHONY : httpserver newHttpserver  clean


newHttpserver:
	gcc -o newHttpserver newHttpserver.c ./http-parser/http_parser.c -luv -pthread -I.

clean:
	rm -rf httpserver
