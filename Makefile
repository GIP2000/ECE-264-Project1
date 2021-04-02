all: compile

PHONY: clean


compile: 
	g++ -std=c++17 main.cpp -o main.out

clean: 
	rm -f main.out
	rm -f output.txt