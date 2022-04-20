.PHONY : main clean

main.ver2:
	g++ -o main main.cpp ./lib/http.cpp -pthread  -I ./lib

main:
	g++ -o main main.cpp ./lib/http.cpp ./lib/registerAPI.cpp -pthread  -I ./lib

clean:
	rm -rf main 
